/**************************************************************************/
/*  rectangle_shape_2d.h                                                  */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#ifndef RECTANGLE_SHAPE_2D_H
#define RECTANGLE_SHAPE_2D_H

#include "scene/resources/2d/shape_2d.h"

class RectangleShape2D : public Shape2D {
	GDCLASS(RectangleShape2D, Shape2D);

	Size2i size;
	Size2i offset;
	void _update_shape();

protected:
	static void _bind_methods();
#ifndef DISABLE_DEPRECATED
	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_property) const;
#endif // DISABLE_DEPRECATED

public:
	void set_size(const Size2i &p_size);
	Size2i get_size() const;

	void set_offset(const Size2i &p_offset);
	Size2i get_offset() const;

	virtual void draw(const RID &p_to_rid, const Color &p_color) override;
	virtual Rect2i get_rect() const override;
	virtual real_t get_enclosing_radius() const override;

	RectangleShape2D();
};

static Vector<Vector2i> rectangle_to_polygon(Size2i size, Point2i offset) {
	Vector<Vector2i> polygon;
	polygon.resize(4);
	polygon.write[0] = offset + -(size / 2);
	polygon.write[1] = offset + Vector2i(size.x + 1, -size.y) / 2;
	polygon.write[2] = offset + (size + Vector2i(1, 1)) / 2;
	polygon.write[3] = offset + Vector2i(-size.x, size.y + 1) / 2;
	return polygon;
}

#endif // RECTANGLE_SHAPE_2D_H
