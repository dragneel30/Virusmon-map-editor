
#pragma once


#include "../JuceLibraryCode/JuceHeader.h"
#include <memory>
#include "Tab.h"
class TabbedComponentWrapper : public ButtonListener
{
    
public:
	TabbedComponentWrapper(TabbedButtonBar::Orientation orientation)
		: tabsHolder(orientation)
	{
	}

	void addTab(String tabName, Tab* tab)
	{
		std::size_t tabCount = tabsHolder.getNumTabs();

		viewports.add(new Viewport("viewport" + std::to_string(tabCount)));

		tabs.add(tab);

		viewports[tabCount]->setViewedComponent(tabs[tabCount], false);

		tabsHolder.addTab(tabName, Colour(123, 123, 123), viewports[tabCount], false, tabCount);
		TextButton* button = new TextButton();
		button->setButtonText("X");
		button->setSize(32, 32);
		button->addListener(this);
		
		tabsHolder.getTabbedButtonBar().getTabButton(tabCount)->setExtraComponent(button, TabBarButton::ExtraComponentPlacement::afterText);


		tabsHolder.addMouseListener(tab, true);
	}



	void buttonClicked(Button* button) override
	{
		for (int a = 0; a < tabsHolder.getNumTabs(); a++)
		{
			if (tabsHolder.getTabbedButtonBar().getTabButton(a)->getExtraComponent() == button)
			{
				tabs.remove(a, true);
				viewports.remove(a, true);
				tabsHolder.removeTab(a);
				return;
			}
		}
	}
	void setBounds(const Rectangle<int>& rect)
	{
		tabsHolder.setBounds(rect);
	}

	TabbedComponent& get()
	{
		return tabsHolder;
	}
private:
	TabbedComponent tabsHolder;
	OwnedArray<Viewport> viewports;
	OwnedArray<Tab> tabs; // each of this is content component of each viewport
};