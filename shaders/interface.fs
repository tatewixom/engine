#version 330 core

out vec4 FragColor;

uniform vec4 color;        // optional, lets you tint/alpha fade the UI

void main()
{
  FragColor = color;
}