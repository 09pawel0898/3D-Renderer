#include "../pch.h"
#include "LabelHolder.h"

void LabelHolder::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& label : mLabels)
		target.draw(label.second);
}

Label& LabelHolder::get_label(ID id)
{
	auto foundLabel = mLabels.find(id);
	assert(foundLabel != mLabels.end());
	return foundLabel->second;
}

const Label& LabelHolder::get_label(ID id) const
{
	auto foundLabel = mLabels.find(id);
	assert(foundLabel != mLabels.end());
	return foundLabel->second;
}
