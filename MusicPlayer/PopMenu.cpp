#include "PopMenu.h"
#include <iostream>

fv::PopMenu::PopMenu() : /*m_pos(0.f, 0.f) ,*/ m_bgColor(sf::Color::Cyan)
{
	m_visible = false;
	m_space = 3;
	m_textColor = sf::Color::White;
	m_hover_Color = sf::Color::Blue;
}

void fv::PopMenu::init(int w, int h, const sf::Font * f)
{
	b_w = w;
	b_h = h;
	n_w = w;
	n_h = h;
	setFont(f);
}

void fv::PopMenu::handlerEvent(sf::Event & event, bool auto_pop)
{
	switch (event.type)
	{
	case sf::Event::MouseButtonReleased:
		if (auto_pop && event.mouseButton.button == sf::Mouse::Right && !m_v.empty())
		{
			//std::cout << event.mouseButton.x << " + " << event.mouseButton.y << std::endl;
			m_visible = true;
			updatePos((float)event.mouseButton.x / n_w * b_w, (float)event.mouseButton.y / n_h * b_h);
		}
		else if (m_visible && event.mouseButton.button == sf::Mouse::Left && !m_v.empty()) {
			for (int i = 0; i < m_v.size(); ++i)
			{
				if (m_v[i]->getGlobalBounds().contains((float)event.mouseButton.x / n_w * b_w, (float)event.mouseButton.y / n_h * b_h))
				{
					if (m_onSelectFunc)
						m_onSelectFunc(i);
					break;
				}
			}
			m_visible = false;
		}
		break;
	case sf::Event::MouseMoved:
		if (m_visible)
		{
			updateBgColor(sf::Vector2f((float)event.mouseMove.x / n_w * b_w, (float)event.mouseMove.y / n_h * b_h));
		}
		break;
	case sf::Event::Resized:
		n_w = (float)event.size.width;
		n_h = (float)event.size.height; 
		break;
	}
}

void fv::PopMenu::draw(sf::RenderWindow & w)
{
	if (m_visible)
	{
		for (auto &t : m_v)
		{
			w.draw(*t);
		}
	}
}

void fv::PopMenu::add(sf::String && str)
{
	if (!font)
		return;
	auto t = std::make_shared<sf::Text>();
	t->setFillColor(m_textColor);
	t->setOutlineThickness(2.0f);
	t->setOutlineColor(m_bgColor);
	t->setFont(*font);
	t->setString(str);
	t->setCharacterSize(18);
	m_v.push_back(t);
}

bool & fv::PopMenu::visible()
{
	return m_visible;
}

void fv::PopMenu::setOnSelected(std::function<void(int)>&& func)
{
	m_onSelectFunc = func;
}

void fv::PopMenu::setFont(const sf::Font * f)
{
	font = const_cast<sf::Font *>(f);
}

void fv::PopMenu::updatePos(float x, float y)
{
	//m_pos.x = x;
	//m_pos.y = y;

	float ky = y;
	for (auto &t : m_v)
	{
		t->setPosition(sf::Vector2f(x, ky));
		ky += t->getGlobalBounds().height;
		ky += m_space;
	}
}

void fv::PopMenu::updateBgColor(sf::Vector2f && p)
{
	for (auto &t : m_v)
	{
		if (t->getGlobalBounds().contains(p))
		{
			//t->setFillColor(m_hover_Color);
			t->setOutlineColor(m_hover_Color);
		}
		else {
			t->setOutlineColor(m_bgColor);
			//t->setFillColor(m);
		}
	}
}