
#pragma once


#include "../JuceLibraryCode/JuceHeader.h"
#include <memory>
#include "Tab.h"
class TabbedComponentWrapper : public ButtonListener
{
    
public:
	TabbedComponentWrapper(TabbedButtonBar::Orientation orientation, std::function<bool(Tab*)> callbacks = std::function<bool(Tab*)>())
		: tabsHolder(orientation), closeTabCallbacks(callbacks)
	{
	}

	void addTab(String tabName, bool hasCloseButton, Tab* tab)
	{
		std::size_t tabCount = tabsHolder.getNumTabs(); //// get before add tabs so it will become like numTab - 1

		tabs.add(tab);

		viewports.add(new Viewport("viewport" + std::to_string(tabCount)));
		viewports[tabCount]->setViewedComponent(tabs[tabCount], false);
		//viewports[tabCount]->addMouseListener(tab, true);

		tabsHolder.addTab(tabName, Colour(123, 123, 123), viewports[tabCount], false, tabCount);

		if (hasCloseButton)
		{
			TextButton* button = new TextButton();
			button->setButtonText("X");
			button->setSize(32, 32);
			button->addListener(this);
			tabsHolder.getTabbedButtonBar().getTabButton(tabCount)->setExtraComponent(button, TabBarButton::ExtraComponentPlacement::afterText);
			
		}
		tabsHolder.setCurrentTabIndex(tabCount);
	}
	
	std::function<bool(Tab*)> closeTabCallbacks;
	void buttonClicked(Button* button) override
	{
		for (int a = 0; a < tabsHolder.getNumTabs(); a++)
		{
			if (tabsHolder.getTabbedButtonBar().getTabButton(a)->getExtraComponent() == button)
			{
				if (closeTabCallbacks(tabs[a]))
				{
					tabs.remove(a, true);
					viewports.remove(a, true);
					tabsHolder.removeTab(a);
				}
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
	OwnedArray<Tab>& getTabs() { return tabs; }

	bool isExist(const String& tabname)
	{
		const StringArray& tabnames = tabsHolder.getTabNames();
		for (int a = 0; a < tabnames.size(); a++)
		{
			if (tabnames[a] == tabname)
				return true;
		}
		return false;
	}

	Tab* getTab(int index)
	{
		return tabs[index];
	}
private:
	TabbedComponent tabsHolder;
	OwnedArray<Viewport> viewports;
	OwnedArray<Tab> tabs; // each of this is content component of each viewport
};