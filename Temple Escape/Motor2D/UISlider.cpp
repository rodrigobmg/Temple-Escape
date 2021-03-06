#include "j1App.h"
#include "UIButton.h"
#include "p2Log.h"
#include "j1Ui.h"
#include "UISlider.h"


UISlider::UISlider(int x, int y, UI_Type type, SDL_Rect slider_rect, j1Module* callback, UIElement* parent) : UIElement(x, y, type, parent)
{
	this->callback = callback;

	this->slider_rect = slider_rect;
	current_rect = slider_rect;

	//LOG("UIButton created in x:%i, y:%i", x, y);
}


void UISlider::SetSliderButtons(UIButton * slider_btn)
{
	this->slider_btn = slider_btn;
}

void UISlider::SetSliderValue(uint slider_btn_pos)
{
	slider_value = slider_btn_pos * 100 / (current_rect.w - slider_btn->GetRect().w);
}

void UISlider::SetSliderValueStart(int slider_value) {
	slider_value = slider_value * 100 / (current_rect.w - slider_btn->GetRect().w);
	this->slider_btn->SetLocalPosition((this->current_rect.w  + 2) * slider_value / 100, 0);
}

uint UISlider::GetSliderValue() const
{
	return slider_value;
}

UIButton * UISlider::GetSliderButton() const
{
	return slider_btn;
}