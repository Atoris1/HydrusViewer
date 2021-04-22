#pragma once
#include "TextBox.hpp"
#include "Tag.hpp"

class TagBox : public TextBox {
private:
	float dy;

public:
	TagBox(SDL_Renderer* rend) :TextBox(rend) {
		color = { 0,0,0 };
		update();
	};

	void addVelocity(int yVelocity) {
		dy += yVelocity;
	};

	void updateMovement() {
		if (abs(dy) > 0.5) {
			for (auto& t_rect : tag_rects) {
				t_rect.y += floor(dy);
			}
			dy -= (dy / 25.f);
		}
	};

	void update() {
		dy = 0;
		tag_rects.clear();

		int ypos = 150;
		int width = 400;
		int height = 30;
		for (int i = 0; i < tags.size(); i++) {

			SDL_QueryTexture(tags[i].getTexture(), NULL, NULL, &width, &height);
			tag_rects.push_back(SDL_Rect({ 0, ypos, width, height }));
			ypos += height + 20;
			
		}
	};

	std::string getTagAtPos(int x, int y) {
		if (x > 400) { return "";}

		for (int i = 0; i < tag_rects.size(); i++) {
			if (y >= tag_rects[i].y && y <= (tag_rects[i].y + tag_rects[i].h)) {
				return tags[i].getTag();
			}
		}

		return "";
	}

	void setTags(std::vector<std::string> new_tags) {
		tags.clear();
		for (auto& tag : new_tags) {
			Tag t = Tag(renderer, tag, font, color, 400);
			t.update();
			tags.push_back(t);
		}
		update();
	};

	void setSelectedTag(std::string call_back_tag) {
		selected_tag = call_back_tag;
	};

	~TagBox()
	{
		for (auto& tag : tags) {
			tag.Destroy();
		}
	}
};