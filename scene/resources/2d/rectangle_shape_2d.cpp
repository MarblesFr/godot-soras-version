/**************************************************************************/
/*  rectangle_shape_2d.cpp                                                */
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

#include "rectangle_shape_2d.h"

#include "servers/physics_server_2d.h"
#include "servers/rendering_server.h"
void RectangleShape2D::_update_shape() {
	Array arr;
	arr.push_back(size);
	arr.push_back(offset);
	PhysicsServer2D::get_singleton()->shape_set_data(get_rid(), arr);
	emit_changed();
}

#ifndef DISABLE_DEPRECATED
bool RectangleShape2D::_set(const StringName &p_name, const Variant &p_value) {
	if (p_name == "size") { // Compatibility with Godot 3.x.
		set_size((Size2i)p_value);
		return true;
	} else if (p_name == "offset") { // Compatibility with Godot 3.x.
		// Convert to `size`, twice as big.
		set_offset((Size2i)p_value);
		return true;
	}
	return false;
}

bool RectangleShape2D::_get(const StringName &p_name, Variant &r_property) const {
	if (p_name == "size") { // Compatibility with Godot 3.x.
		r_property = size;
		return true;
	} else if (p_name == "offset") { // Compatibility with Godot 3.x.
		// Convert to `extents`, half as big.
		r_property = offset;
		return true;
	}
	return false;
}
#endif // DISABLE_DEPRECATED

void RectangleShape2D::set_size(const Size2i &p_size) {
	ERR_FAIL_COND_MSG(p_size.x < 0 || p_size.y < 0, "RectangleShape2D size cannot be negative.");
	size = p_size;
	_update_shape();
}

Size2i RectangleShape2D::get_size() const {
	return size;
}

void RectangleShape2D::set_offset(const Size2i &p_offset) {
	offset = p_offset;
	_update_shape();
}

Size2i RectangleShape2D::get_offset() const {
	return offset;
}

void RectangleShape2D::draw(const RID &p_to_rid, const Color &p_color) {
	RenderingServer::get_singleton()->canvas_item_add_rect(p_to_rid, Rect2(offset - (size / 2), size), p_color);
	if (is_collision_outline_enabled()) {
		// Draw an outlined rectangle to make individual shapes easier to distinguish.
		Vector<Vector2> stroke_points;
		stroke_points.resize(5);
		stroke_points.write[0] = offset + -(size / 2);
		stroke_points.write[1] = offset + Vector2i(size.x + 1, -size.y) / 2;
		stroke_points.write[2] = offset + (size + Vector2i(1, 1)) / 2;
		stroke_points.write[3] = offset + Vector2i(-size.x, size.y + 1) / 2;
		stroke_points.write[4] = offset + -(size / 2);

		Vector<Color> stroke_colors = { Color(p_color, 1.0) };

		RenderingServer::get_singleton()->canvas_item_add_polyline(p_to_rid, stroke_points, stroke_colors);
	}
}

Rect2i RectangleShape2D::get_rect() const {
	return Rect2i(-(size / 2) + offset, size);
}

real_t RectangleShape2D::get_enclosing_radius() const {
	return size.length() / 2;
}

void RectangleShape2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_size", "size"), &RectangleShape2D::set_size);
	ClassDB::bind_method(D_METHOD("get_size"), &RectangleShape2D::get_size);
	ClassDB::bind_method(D_METHOD("set_offset", "offset"), &RectangleShape2D::set_offset);
	ClassDB::bind_method(D_METHOD("get_offset"), &RectangleShape2D::get_offset);

	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "size", PROPERTY_HINT_NONE, "suffix:px"), "set_size", "get_size");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "offset", PROPERTY_HINT_NONE, "suffix:px"), "set_offset", "get_offset");
}

RectangleShape2D::RectangleShape2D() :
		Shape2D(PhysicsServer2D::get_singleton()->rectangle_shape_create()) {
	size = Size2i(20, 20);
	offset = Size2i(0, 0);
	_update_shape();
}
