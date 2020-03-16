#pragma once
enum viewtype { top_view, perspective_view, rat_view };
extern viewtype current_view;
void DrawLine(double x1, double y1, double x2, double y2);
void SetTopView(int w, int h);
void SetPerspectiveView(int w, int h);
