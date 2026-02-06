#pragma once

class Window;

class Mouse
{
public:
  struct Position
  {
    double x{};
    double y{};

    friend bool operator==(const Position& pos1, const Position& pos2);
    friend Position operator-(const Position& pos1, const Position& pos2);
  };

  struct Button
  {
    int button{};
    int action{};
  };

  Mouse(Window& window);

  void update();

  const Position& position() const noexcept { return position_; }
  const Position& poffset() const noexcept { return offset_; }
  const Position& lposition() const noexcept { return lastPosition_; }
  const Button& button() const noexcept { return button_; }
  const Position& soffset() const noexcept { return scrollOffset_; }

  void position(Position p) noexcept { position_ = p; }
  void poffset(Position p) noexcept { offset_ = p; }
  void lposition(Position p) noexcept { lastPosition_ = p; }
  void button(Button b) noexcept { button_ = b; }
  void soffset(Position p) noexcept { scrollOffset_ = p; }

  //mouse manipulators
  void center();
  void viewMode();
  void selectionMode();
  void setLastPosition(); //sets last position from being in view mode

  //conditionals
  bool isButtonPressed(int mouseButton) const;
  bool isDisabled() const;
  bool isMoving() const noexcept { return position_ != lastPosition_; }

private:
  Window& window_;
  Position position_{};
  Position offset_{};
  Position lastPosition_{};
  Button button_{};
  Position scrollOffset_{};
};

