/**************************************************************************/
/*  character_body_2d.cpp                                                 */
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

#include "character_body_2d.h"

void CharacterBody2D::_bind_methods() {
	GDVIRTUAL_BIND(_is_riding_solid, "solid");
	GDVIRTUAL_BIND(_is_riding_one_way, "one_way");
	GDVIRTUAL_BIND(_squish);

	ClassDB::bind_method(D_METHOD("set_ignores_one_way", "enabled"), &CharacterBody2D::set_ignores_one_way);
	ClassDB::bind_method(D_METHOD("is_ignores_one_way_enabled"), &CharacterBody2D::is_ignores_one_way_enabled);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "ignores_one_way"), "set_ignores_one_way", "is_ignores_one_way_enabled");
}

CharacterBody2D::CharacterBody2D() :
		PhysicsBody2D(PhysicsServer2D::BODY_MODE_KINEMATIC, PhysicsServer2D::COLLIDER_TYPE_ACTOR) {
	PhysicsServer2D::get_singleton()->body_set_is_riding_solid(get_rid(), callable_mp(this, &CharacterBody2D::_is_riding_solid));
	PhysicsServer2D::get_singleton()->body_set_is_riding_one_way(get_rid(), callable_mp(this, &CharacterBody2D::_is_riding_one_way));
}

bool CharacterBody2D::move_h_exact(int32_t p_amount, const Callable &p_callback) {
	int move_dir = SIGN(p_amount);
	Vector2i move_dir_vector = Vector2i(move_dir, 0);
	int amount_moved = 0;
	PhysicsServer2D::CollisionResult r_result;
	while (p_amount != 0)
	{
		bool colliding = collides_at(move_dir_vector, &r_result);
		if (colliding)
		{
			position_delta.x = 0;
			if (p_callback.is_valid())
			{
				p_callback.call(move_dir_vector, amount_moved, p_amount, r_result.collider);
			}
			return true;
		}
		amount_moved += move_dir;
		p_amount -= move_dir;
		translate(move_dir_vector);
	}
	return false;
}

bool CharacterBody2D::move_v_exact(int32_t p_amount, const Callable &p_callback) {
	int move_dir = SIGN(p_amount);
	Vector2i move_dir_vector = Vector2i(0, move_dir);
	int amount_moved = 0;
	PhysicsServer2D::CollisionResult r_result;
	while (p_amount != 0)
	{
		bool colliding = collides_at(move_dir_vector, &r_result);
		if (colliding)
		{
			position_delta.y = 0;
			if (p_callback.is_valid())
			{
				p_callback.call(move_dir_vector, amount_moved, p_amount, r_result.collider);
			}
			return true;
		}
		if (p_amount > 0 && !ignores_one_way) {
			colliding = collides_at_outside(move_dir_vector, &r_result, PhysicsServer2D::COLLIDER_TYPE_ONE_WAY);
			if (colliding)
			{
				position_delta.y = 0;
				if (p_callback.is_valid())
				{
					p_callback.call(move_dir_vector, amount_moved, p_amount, r_result.collider);
				}
				return true;
			}
		}
		amount_moved += move_dir;
		p_amount -= move_dir;
		translate(move_dir_vector);
	}
	return false;
}

bool CharacterBody2D::on_ground() {
	return collides_at(Vector2i(0, 1)) || (!ignores_one_way && collides_at_outside(Vector2i(0, 1), nullptr, PhysicsServer2D::COLLIDER_TYPE_ONE_WAY));
}

void CharacterBody2D::set_ignores_one_way(bool p_enable) {
	ignores_one_way = p_enable;
}

bool CharacterBody2D::is_ignores_one_way_enabled() const {
	return ignores_one_way;
}

bool CharacterBody2D::_is_riding_solid(const RID &p_solid) {
	bool result = false;
	if(GDVIRTUAL_CALL(_is_riding_solid, p_solid, result)) {
		return result;
	}
	result = collides_at_with(Vector2i(0, 1), p_solid);
	return result;
}

bool CharacterBody2D::_is_riding_one_way(const RID &p_one_way) {
	bool result = false;
	if(GDVIRTUAL_CALL(_is_riding_one_way, p_one_way, result)) {
		return result;
	}
	result = !ignores_one_way && collides_at_with_outside(Vector2i(0, 1), p_one_way);
	return result;
}

void CharacterBody2D::_squish() {
	GDVIRTUAL_CALL(_squish);
}
