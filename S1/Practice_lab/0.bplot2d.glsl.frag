#version 120
// sample BPlot2D shader

// shader parameters

uniform vec2 screen; // screen pixel sizes
uniform vec4 world;  // box rectangle: (x0, y0, x1-x0, y1-y0)


// indicator function

// Проверить попадание в круг.
bool in_circle(float x, float y, // координаты точки
  float cx, float cy, float r)   // координаты центра круга и его радиус
{
  // Константы -- после инициализации значения не изменяются.
  float dx = x - cx,
        dy = y - cy;

  return dx * dx + dy * dy <= r * r;
}

// Проверить попадание в ромб.
bool in_rhomb(float x, float y, // координаты точки
  float cx, float cy, float rx, float ry)   // координаты центра ромба и его радиус
{
  // Константы -- после инициализации значения не изменяются.
  float dx = x - cx,
        dy = y - cy;

  return (rx * abs(dx) + ry * abs(dy) <= rx * ry );
}

// Проверить попадание в заданную фигуру.
bool indicator(vec2 p)
{
    bool a = in_circle(p.x, p.y, 0.0, 1.0, 1.0),
         b = in_rhomb(p.x, p.y, 0, 0, 1.0, 1.0);
    
  return ( (a && !b) || (!a && b) );
}


// auxiliary functions

// maps pixel screen coordinates pc to "world" coordinates
vec2 screen_to_world(vec2 pc)
{
  return world.xy + pc / screen * world.zw;
}


// colors to paint
const vec3 fill_color = vec3(0.1, 0.5, 0.4);
		   
// supersampling
const int offsets = 13;
vec2 off[offsets] = vec2[]
(
  vec2(0.50, 0.50),
  vec2(0.40, 0.65),
  vec2(0.65, 0.60),
  vec2(0.60, 0.35),
  vec2(0.35, 0.40),
  vec2(0.15, 0.80),
  vec2(0.80, 0.85),
  vec2(0.85, 0.20),
  vec2(0.20, 0.15),
  vec2(0.10, 0.45),
  vec2(0.45, 0.90),
  vec2(0.90, 0.55),
  vec2(0.55, 0.10)
);
		   
void main(void)
{
  float hits = 0.0;
  for (int i = 0; i < offsets; ++i)
    hits += (indicator(screen_to_world(gl_FragCoord.xy + off[i]))? 1.0: 0.0);

  gl_FragColor = vec4(fill_color, hits / float(offsets));
}
