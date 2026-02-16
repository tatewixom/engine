#include "Model.h"
#include "Shader.h"

#include <variant>

namespace Nuke
{
  using DataPointer = std::variant<const std::uint8_t *, const std::uint16_t *, const std::uint32_t *>;

  static DataPointer getAttributeData(const tinygltf::Model &model, const tinygltf::Accessor &accessor)
  {
    const tinygltf::BufferView &bufferView = model.bufferViews[accessor.bufferView];
    const tinygltf::Buffer &buffer = model.buffers[bufferView.buffer];

    const std::uint8_t *data = buffer.data.data() + bufferView.byteOffset + accessor.byteOffset;

    switch (accessor.componentType)
    {
    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
      return data;
    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
      return reinterpret_cast<const std::uint16_t *>(data);
    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
      return reinterpret_cast<const std::uint32_t *>(data);
    default:
      std::cerr << "ERROR::MODEL.CPP::GETATTRIBUTEDATA()::UNSUPPORTED_COMPONENT_TYPE\n";
      throw std::runtime_error("UNSUPPORTED_COMPONENT_TYPE");
    }
  }

  template <typename T>
  const T *GetAttributeData(const tinygltf::Model &model, const tinygltf::Accessor &accessor)
  {
    const tinygltf::BufferView &bufferView = model.bufferViews[accessor.bufferView];
    const tinygltf::Buffer &buffer = model.buffers[bufferView.buffer];

    const unsigned char *dataPtr = buffer.data.data() + bufferView.byteOffset + accessor.byteOffset;

    return reinterpret_cast<const T *>(dataPtr);
  }

  static tinygltf::Model make_gltf_model(std::string_view gltf_file_path)
  {
    // detect file extension
    bool isBinary{false};
    constexpr std::size_t minExtensionSize{4};
    if (gltf_file_path.size() >= minExtensionSize)
    {
      std::string extension{gltf_file_path.substr(gltf_file_path.find_last_of('.') + 1)};
      for (auto &c : extension)
        c = static_cast<char>(std::tolower(static_cast<int>(c)));

      isBinary = (extension == "glb");
    }

    bool ret{false};
    tinygltf::TinyGLTF loader{};
    tinygltf::Model model{};

    // error msgs
    std::string err{};
    std::string warn{};

    if (isBinary)
      ret = loader.LoadBinaryFromFile(&model, &err, &warn, gltf_file_path.data());
    else
      ret = loader.LoadASCIIFromFile(&model, &err, &warn, gltf_file_path.data());

    // warnings & errors
    if (!warn.empty())
      std::cerr << "[WARNING]: " << warn << '\n';
    if (!err.empty())
      std::cerr << "[ERROR]: " << err << '\n';
    if (!ret)
      std::cerr << "[ERROR]: Failure in loading model: " << gltf_file_path << '\n';

    return model;
  }

  std::vector<Vertex> load_vertices(const tinygltf::Model &model, const tinygltf::Primitive &primitive)
  {
    std::vector<Vertex> vertices{};

    /* position accessor */
    const tinygltf::Accessor &posAccessor{model.accessors[primitive.attributes.at("POSITION")]};
    const glm::vec3 *positions{GetAttributeData<glm::vec3>(model, posAccessor)};

    if (!positions)
      throw std::runtime_error("ERROR::MESH_MUST_HAVE_POSITIONS");

    /* normals accessor */
    const glm::vec3 *normals{nullptr};

    if (primitive.attributes.count("NORMAL"))
    {
      const tinygltf::Accessor &normAccessor{model.accessors[primitive.attributes.at("NORMAL")]};
      normals = GetAttributeData<glm::vec3>(model, normAccessor);
    }

    /* texture coords accessor */
    const glm::vec2 *texcoords{nullptr};

    if (primitive.attributes.count("TEXCOORD_0"))
    {
      const tinygltf::Accessor &texAccessor = model.accessors[primitive.attributes.at("TEXCOORD_0")];
      texcoords = GetAttributeData<glm::vec2>(model, texAccessor);
    }

    /* extracting position, texture, and normal accessors */
    size_t positionsCount{posAccessor.count};
    vertices.resize(positionsCount);

    // extracting positions. MUST HAVE POSITIONS
    for (size_t k{0}; k < positionsCount; ++k)
      vertices[k].position = positions[k];

    // extracting normals (optional)
    if (normals)
      for (size_t k{0}; k < positionsCount; ++k)
        vertices[k].normal = normals[k];

    // extracting texture coords (optional)
    if (texcoords)
      for (size_t k{0}; k < positionsCount; ++k)
        vertices[k].texture = texcoords[k];

    return vertices;
  }

  template <typename T>
  std::vector<T> load_indices(const tinygltf::Model &model, const tinygltf::Primitive &primitive)
  {
    std::vector<T> indices{};

    const tinygltf::Accessor &indexAccessor{model.accessors[primitive.indices]};

    auto indiceCount{indexAccessor.count};
    indices.resize(indiceCount);

    const T *indexData = GetAttributeData<T>(model, indexAccessor);

    for (std::size_t j{0}; j < indiceCount; ++j)
      indices[j] = indexData[j];

    return indices;
  }

  GLenum get_indice_type(const tinygltf::Accessor &indiceAccessor)
  {
    switch (indiceAccessor.componentType)
    {
    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
      return GL_UNSIGNED_BYTE;
    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
      return GL_UNSIGNED_SHORT;
    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
      return GL_UNSIGNED_INT;
    default:
      throw std::runtime_error("ERROR::UNSUPPORTED_DATA_TYPE");
    }
  }

  std::vector<Texture> load_textures(const tinygltf::Model &model, const tinygltf::Primitive &primitive)
  {
    std::vector<Texture> textures{};

    int materialIndex{primitive.material};
    const tinygltf::Image *image{};
    if (materialIndex > 0)
    {
      const tinygltf::Material &mat = model.materials[materialIndex];

      // extracting base color
      int baseColorTexIndex{mat.pbrMetallicRoughness.baseColorTexture.index};
      if (baseColorTexIndex >= 0)
      {
        const tinygltf::Texture &tex{model.textures[baseColorTexIndex]};
        int imageIndex{tex.source};

        image = &model.images[imageIndex];
      }

      if (image)
      {
        Texture temptex{};
        temptex.initialize(image->image.data(), image->width, image->height, image->mimeType, Texture::Type::base);
        textures.push_back(temptex);
      }

      // extracting normal map
      image = nullptr;
      int normalTexIndex = mat.normalTexture.index;
      if (normalTexIndex >= 0)
      {
        const tinygltf::Texture &tex = model.textures[normalTexIndex];
        image = &model.images[tex.source];
      }

      if (image)
      {
        Texture temptex{};
        temptex.initialize(image->image.data(), image->width, image->height, image->mimeType, Texture::Type::normal);
        textures.push_back(temptex);
      }
    }

    return textures;
  }

  std::vector<Mesh> load_meshes(tinygltf::Model &model)
  {
    std::vector<Mesh> meshes{};

    for (auto &mesh : model.meshes)
    {
      std::vector<Vertex> vertices{};
      std::vector<std::uint32_t> indices{};
      std::vector<Texture> textures{};
      GLenum indexType{};

      for (auto &primitive : mesh.primitives)
      {
        /* load vertices */
        /*
          1. load positions
          2. load normals (optional)
          3. load texture coords (optional)
        */

        vertices = load_vertices(model, primitive);

        /* load indices */
        /*
          1. load indices with correct byte size
          2. get type information
        */

        indices = load_indices<std::uint32_t>(model, primitive);

        /* load textures */

        textures = load_textures(model, primitive);
      }

      Mesh tempMesh{};
      tempMesh.initialize(vertices, indices,
                          std::vector<Attribute>{
                              Attribute{0u, 3, sizeof(Vertex), offsetof(Vertex, Vertex::position)},
                              Attribute{2u, 2, sizeof(Vertex), offsetof(Vertex, Vertex::texture)},
                              Attribute{3u, 3, sizeof(Vertex), offsetof(Vertex, Vertex::normal)}},
                          std::move(textures));

      /*
        GOAL FOR NEXT SESSION:
        get to work on making the function more general as to work with the three
        supported index types.

        this could be achieved by possibly adding three paths depending on a switch
        case to a for loop that uses the right index type. It sucks because it
        seems that template metaprogramming doesn't work well here and only leads
        to more issues.
      */

      tempMesh.changeIndexType(GL_UNSIGNED_INT);

      meshes.push_back(tempMesh);
    }

    return meshes;
  }

  void Model::loadMeshes(tinygltf::Model &model)
  {
    // loading meshes from model file
    for (std::size_t i{0}; i < model.meshes.size(); ++i)
    {
      const tinygltf::Mesh &mesh{model.meshes[i]};

      std::vector<Vertex> vertices{};
      std::vector<std::uint16_t> indices{};
      std::vector<Texture> textures{};
      GLenum indexType{};

      for (auto &primitive : mesh.primitives)
      {
        /* position accessor */
        const tinygltf::Accessor &posAccessor{model.accessors[primitive.attributes.at("POSITION")]};
        const glm::vec3 *positions{GetAttributeData<glm::vec3>(model, posAccessor)};

        size_t positionsCount{posAccessor.count};

        /* normals accessor */
        const glm::vec3 *normals{nullptr};

        if (primitive.attributes.count("NORMAL"))
        {
          const tinygltf::Accessor &normAccessor{model.accessors[primitive.attributes.at("NORMAL")]};
          normals = GetAttributeData<glm::vec3>(model, normAccessor);
        }

        /* texture coords accessor */
        const glm::vec2 *texcoords{nullptr};

        if (primitive.attributes.count("TEXCOORD_0"))
        {
          const tinygltf::Accessor &uvAccessor =
              model.accessors[primitive.attributes.at("TEXCOORD_0")];
          texcoords = GetAttributeData<glm::vec2>(model, uvAccessor);
        }

        /* extracting position, texture, and normal accessors */
        vertices.resize(positionsCount);

        // extracting positions. MUST HAVE POSITIONS OR PROGRAM WILL TERMINATE
        if (positions)
          for (size_t k{0}; k < positionsCount; ++k)
            vertices[k].position = positions[k];
        else
          throw std::runtime_error("ERROR::MESH_MUST_HAVE_POSITIONS");

        // extracting normals (optional)
        if (normals)
          for (size_t k{0}; k < positionsCount; ++k)
            vertices[k].normal = normals[k];

        // extracting texture coords (optional)
        if (texcoords)
          for (size_t k{0}; k < positionsCount; ++k)
            vertices[k].texture = texcoords[k];

        /*
          1. Omitted checking for null pointers on every for-iteration.
          2. Omitted manually setting values for Vertex members if pointer
          was null. That is:
          if (normals)
            ...
          else
            vertices[i].position = glm::vec3(0.f);

          Also reconstructed the for-loops to seperately do normals,
          texcoords, and poisitons respectively.

          THIS ASSUMES THAT THE VECTOR VALUE INITIALIZES VERTEX's DATA MEMBERS
        */

        /* indices accessor */
        const tinygltf::Accessor &indexAccessor{model.accessors[primitive.indices]};

        auto indiceCount{indexAccessor.count};
        indices.resize(indiceCount);

        /* extracting indices */
        // determining index type via switch
        // get pointer to index type
        switch (indexAccessor.componentType)
        {
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
        {
          indexType = GL_UNSIGNED_BYTE;

          const std::uint8_t *indexData = GetAttributeData<std::uint8_t>(model, indexAccessor);

          for (std::size_t j{0}; j < indiceCount; ++j)
            indices[j] = indexData[j];
          break;
        }
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
        {
          indexType = GL_UNSIGNED_SHORT;

          const std::uint16_t *indexData = GetAttributeData<std::uint16_t>(model, indexAccessor);

          for (std::size_t j{0}; j < indiceCount; ++j)
            indices[j] = indexData[j];
          break;
        }
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
        {
          /*
            need to fix adding making three different types of vectors to send indices.
            the current code only supports unsigned shorts, so model like the alien
            load incorrectly.
          */
          indexType = GL_UNSIGNED_INT;

          const std::uint32_t *indexData = GetAttributeData<std::uint32_t>(model, indexAccessor);

          for (std::size_t j{0}; j < indiceCount; ++j)
            indices[j] = indexData[j];
          break;
        }
        default:
        {
          throw std::runtime_error("ERROR::UNSUPPORTED_DATA_TYPE");
          break;
        }
        }

        /* extracting textures */
        int materialIndex{primitive.material};
        const tinygltf::Image *image{};
        if (materialIndex >= 0)
        {
          const tinygltf::Material &mat = model.materials[materialIndex];

          // extracting base color texture
          int baseColorTexIndex{mat.pbrMetallicRoughness.baseColorTexture.index};
          if (baseColorTexIndex >= 0)
          {
            const tinygltf::Texture &tex{model.textures[baseColorTexIndex]};
            int imageIndex{tex.source};

            image = &model.images[imageIndex];
          }

          if (image)
          {
            Texture temptex{};
            temptex.initialize(image->image.data(), image->width, image->height, image->mimeType, Texture::Type::base);
            textures.push_back(temptex);
          }

          // extracting normal map
          image = nullptr;
          int normalTexIndex = mat.normalTexture.index;
          if (normalTexIndex >= 0)
          {
            const tinygltf::Texture &tex = model.textures[normalTexIndex];
            image = &model.images[tex.source];
          }

          if (image)
          {
            Texture temptex{};
            temptex.initialize(image->image.data(), image->width, image->height, image->mimeType, Texture::Type::normal);
            textures.push_back(temptex);
          }

          /*
          //extracting metallic roughness
          image = nullptr;
          int mrTexIndex = mat.pbrMetallicRoughness.metallicRoughnessTexture.index;
          if (mrTexIndex >= 0)
          {
            const tinygltf::Texture& tex{ model.textures[mrTexIndex] };
            int imageIndex{ tex.source };
            image = &model.images[imageIndex];
          }

          if (image)
          {
            Texture temptex{};
            temptex.initialize(image->image.data(), image->width, image->height, image->mimeType);
            textures.push_back(temptex);
          }
          */
        }
      }

      // creating and initializing temporary Mesh object
      Mesh temp{};
      temp.initialize(vertices, indices,
                      std::vector<Attribute>{
                          Attribute{0u, 3, sizeof(Vertex), offsetof(Vertex, Vertex::position)},
                          Attribute{2u, 2, sizeof(Vertex), offsetof(Vertex, Vertex::texture)},
                          Attribute{3u, 3, sizeof(Vertex), offsetof(Vertex, Vertex::normal)}},
                      std::move(textures));
      temp.changeIndexType(indexType);
      meshes_.emplace_back(std::move(temp));

      // preparing vectors for next mesh run
      vertices.clear();
      indices.clear();
      textures.clear();
    }
  }

  void Model::loadNodes(tinygltf::Model &model)
  {
    nodes_.resize(model.nodes.size());

    for (std::size_t i{0}; i < model.nodes.size(); ++i)
      nodes_[i] = convert_to_node(model.nodes[i]);
  }

  Node Model::convert_to_node(tinygltf::Node &node)
  {
    if (!node.matrix.empty())
      return Node{node.mesh, std::move(node.children), vec_to_mat(node.matrix)};
    else
    {
      glm::mat4 local{1.f};

      if (!node.translation.empty())
        local = glm::translate(local, glm::vec3{node.translation[0], node.translation[1], node.translation[2]});

      if (!node.rotation.empty())
      {
        glm::quat q{
            static_cast<float>(node.rotation[3]),
            static_cast<float>(node.rotation[0]),
            static_cast<float>(node.rotation[1]),
            static_cast<float>(node.rotation[2])};

        local *= glm::mat4_cast(q);
      }

      if (!node.scale.empty())
        local = glm::scale(local, glm::vec3{node.scale[0], node.scale[1], node.scale[2]});

      return Node{node.mesh, std::move(node.children), std::move(local)};
    }
  }

  glm::mat4 Model::vec_to_mat(const std::vector<double> &vec) noexcept
  {
    if (vec.size() == 0)
      return glm::mat4{1.0f};

    if (vec.size() != 16)
    {
      std::cerr << "ERROR::MODEL.CPP::VEC_TO_MAT()::SIZE_MUST_BE_SIXTEEN\n";
      return glm::mat4{1.0f};
    }

    glm::mat4 mat{1.0f};

    // Fill in column-major order
    for (std::size_t col{0}; col < 4; ++col)
      for (std::size_t row{0}; row < 4; ++row)
        mat[col][row] = static_cast<float>(vec[(col * 4) + row]);

    return mat;
  }

  void Model::loadRoots(tinygltf::Model &model)
  {
    auto &scene{model.scenes[model.defaultScene]};
    int rootIndex{static_cast<int>(nodes_.size())};
    nodes_.push_back(Node{-1, scene.nodes, glm::mat4{1.f}});

    roots_.push_back(rootIndex);
  }

  void Model::initialize_t(std::string_view gltf_file_path, glm::vec3 &&position)
  {
    /* set position */
    position_ = position;

    /* import */
    tinygltf::Model model{make_gltf_model(gltf_file_path)};

    if (model.meshes.size() < 1)
      return;

    /* convert */
    loadMeshes(model);
    loadNodes(model);
    loadRoots(model);

    /* discard (implicit) */
  }

  void Model::drawNode(int nodeIndex, const glm::mat4 &parentWorld, Shader &shader)
  {
    const Node &node = nodes_[nodeIndex];

    glm::mat4 world = parentWorld * node.local;

    if (node.meshIndex >= 0)
    {
      shader.activate();
      shader.set("model", world);
      meshes_[node.meshIndex].draw(shader);
    }

    for (int child : node.children)
      drawNode(child, world, shader);
  }

  void Model::draw(Shader &shader)
  {
    glm::mat4 identity{1.0f};

    identity = glm::translate(identity, position_);
    identity = glm::scale(identity, glm::vec3{1.0f, 1.0f, 1.0f});

    for (int root : roots_)
      drawNode(root, identity, shader);
  }
}