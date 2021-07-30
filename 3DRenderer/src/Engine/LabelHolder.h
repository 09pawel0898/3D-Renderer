#pragma once

typedef sf::Text Label;

class LabelHolder : public sf::Drawable
{
public:
	enum class ID
	{
		INFO,
		NUM_MESH_POLYGONS,
		NUM_CLIPPED_TRIS,
		NUM_DRAWN_TRIS
	};

private:
	std::unordered_map<ID,Label> mLabels;

public:
	LabelHolder() {}

	virtual void draw(sf::RenderTarget& target,sf::RenderStates states) const override;
	inline void add_label(ID id, const vec2f&& labelPos, sf::Text&& text);
	
	Label& get_label(ID id);
	const Label& get_label(ID id) const;
	Label& operator[](ID id) {	return get_label(id);	}
	const Label& operator[](ID id) const { return get_label(id); }
};


inline void LabelHolder::add_label(ID id, const vec2f&& labelPos, sf::Text&& text)
{
	text.setPosition(labelPos);
	mLabels.insert(std::make_pair(id, Label(text)));
}

