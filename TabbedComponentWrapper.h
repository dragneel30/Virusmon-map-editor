
#pragma once


#include "../JuceLibraryCode/JuceHeader.h"
#include <memory>
#include "Tab.h"
class TabbedComponentWrapper
{
    
public:
	TabbedComponentWrapper(TabbedButtonBar::Orientation orientation)
		: tabsHolder(orientation)
	{
		
	}

	void addTab(String tabName, ScopedPointer<Tab> tab)
	{
		std::size_t tabCount = tabsHolder.getNumTabs();

		viewports.emplace_back(new Viewport("unkown"));
			
		tabs.emplace_back(tab);
		
		viewports[tabCount]->setViewedComponent(tabs[tabCount], true);

		tabsHolder.addTab(tabName, Colour(123, 123, 123), viewports[tabCount], false, tabCount);
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
	std::vector<ScopedPointer<Viewport>> viewports;
	std::vector<ScopedPointer<Tab>> tabs; // each of this is content component of each viewport
};