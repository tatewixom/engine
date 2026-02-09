#pragma once

#include "Mesh.h"

#include <tiny_gltf.h>
#include <stb_image.h>

struct Node
{
  int meshIndex{};
  std::vector<int> children{};
  glm::mat4 local{};
};

class Model
{
public:
  void initialize(std::string_view gltf_file_path);
  void initialize_t(std::string_view gltf_file_path, glm::vec3&& position);

  void loadMeshes(tinygltf::Model& model);

  void loadNodes(tinygltf::Model& model);
  Node convert_to_node(tinygltf::Node& node);

  glm::mat4 vec_to_mat(const std::vector<double>& vec) noexcept;

  void loadRoots(tinygltf::Model& model);

  void drawNode(int nodeIndex, const glm::mat4& parentWorld, Shader& shader);
  void draw(Shader& shader);

private:
  std::vector<Mesh> meshes_{};
  std::vector<Node> nodes_{};
  std::vector<int> roots_{}; //scene root nodes
  std::vector<Texture> textures_{};
  glm::vec3 position_{};
};