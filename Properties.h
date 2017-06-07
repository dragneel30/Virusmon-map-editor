/*
  ==============================================================================

    Properties.h
    Created: 18 May 2017 6:04:22am
    Author:  Lorence

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include <vector>
#include "PropertiesWindow.h"
class PropertiesModal : public Modal
{
public:
	PropertiesModal(std::pair<String, String> pair = std::pair<String, String>())
		: Modal(Vector2i(200, 96))
	{
		setSize(200, 96);
		addAndMakeVisible(property);
		addAndMakeVisible(value);
		addAndMakeVisible(txtProperty);
		addAndMakeVisible(txtValue);

		property.setText("Property:", NotificationType::dontSendNotification);
		value.setText("Value: ", NotificationType::dontSendNotification);
		success.setButtonText("Save");
		cancel.setButtonText("Cancel");
		if (pair != std::pair<String, String>())
		{
			myLog(pair.first.toStdString());
			txtProperty.setText(pair.first);
			txtValue.setText(pair.second);
			txtProperty.setEnabled(false);
		}

		int height = 32;
		int width = 100;
		property.setBounds(0, 0, width, height);
		value.setBounds(0, height, width, height);
		txtProperty.setBounds(width, 0, width, height);
		txtValue.setBounds(width, height, width, height);
		success.setBounds(0, value.getHeight() + height, width, height);
		cancel.setBounds(width, value.getHeight() + height, width, width);
	}


	std::pair<String, String> get()
	{
		return std::make_pair(txtProperty.getText(), txtValue.getText());
	}
	Label property;
	Label value;
	TextEditor txtProperty;
	TextEditor txtValue;
};
class PropertiesWindow;
class Properties
{
public:
	Properties()
		: edited(false), defaultPropertiesCount(0)
	{
	
	}

	void add(std::string property, std::string value)
	{
		add(std::make_pair(property, value));
	}
	void add(std::pair<String, String> property)
	{
		strProperties.push_back(property);
		setEdited(true);
	}

	std::vector<std::pair<String, String>>& get()
	{
		return strProperties;
	}
	const std::vector<std::pair<String, String>>& getProperties()
	{
		return strProperties;
	}
	void show();
	std::size_t getCount() const
	{
		return strProperties.size();
	}

	void setEdited(bool pEdited) { edited = pEdited; }
	bool isEdited() const { return edited; }
	int getDefaultPropertiesCount() const { return defaultPropertiesCount; }
	void setDefaultPropertiesCount(int count) { defaultPropertiesCount = count; }
private:
	bool edited;
	int defaultPropertiesCount;
	static PropertiesWindow* window;
	std::vector<std::pair<String, String>> strProperties;
	
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Properties)
};


class PropertiesWindow : public BaseWindow, public ButtonListener
{
public:
	virtual void closeButtonPressed() override
	{
		setVisible(false);
	}
	virtual ~PropertiesWindow()
	{

	}
	PropertiesWindow(const String& name, Vector2i size, Properties& properties)
		: BaseWindow(name, size), contentComponent(new Component()), header(new TableHeaderComponent()), model(properties),
		table(String(), &model)
	{

		setContentOwned(contentComponent, false);
		model.setButtonToTriggerWhenCellDoubleClicked(&edit);
		header->addColumn("Value", 2, 100, 0, 100, TableHeaderComponent::ColumnPropertyFlags::defaultFlags, 0);
		header->addColumn("Property", 1, 100, 0, 100, TableHeaderComponent::ColumnPropertyFlags::defaultFlags, 0);

		table.setHeader(header);
		Colour c((uint8)123, (uint8)123, (uint8)123, 0.5f);
		add.setImages(false, true, true, getImageFromFile("Asset/add.png"), 1.0f, Colour(), Image(), 1.0f, c, Image(), 1.0f, c);
		remove.setImages(false, true, true, getImageFromFile("Asset/remove.png"), 1.0f, Colour(), Image(), 1.0f, c, Image(), 1.0f, c);
		edit.setImages(false, true, true, getImageFromFile("Asset/edit.png"), 1.0f, Colour(), Image(), 1.0f, c, Image(), 1.0f, c);

		contentComponent->addAndMakeVisible(table);
		contentComponent->addAndMakeVisible(add);
		contentComponent->addAndMakeVisible(remove);
		contentComponent->addAndMakeVisible(edit);

		add.addListener(this);
		remove.addListener(this);
		edit.addListener(this);
	}
	TableListBox& getTable() { return table; }
	void buttonClicked(Button* button) override
	{
		if (button == &add)
		{
			PropertiesModal addmodal;
			if (openModal(addmodal) == 69)
			{
				if (model.addRow(addmodal.get()))
					table.updateContent();
				else
					table.repaintRow(model.getLastUpdate());
			}
		}
		else if (button == &edit)
		{
			if (!table.getSelectedRows().isEmpty())
			{
				int selectedRow = table.getSelectedRow();
				PropertiesModal editmodal(model.getRow(selectedRow));
				if (openModal(editmodal) == 69)
				{
					model.editValue(selectedRow, editmodal.get().second);
					table.repaintRow(selectedRow);
				}

			}
		}
		else if (button == &remove)
		{
			if (!table.getSelectedRows().isEmpty())
			{
				model.remove(table.getSelectedRow());
				table.updateContent();
			}
		}
	}
	void resized() override
	{
		BaseWindow::resized();
		Rectangle<int> bounds(contentComponent->getLocalBounds());

		int width = 16;
		int height = 16;
		table.setBounds(bounds.removeFromTop(bounds.getHeight() - height));
		add.setBounds(bounds.removeFromLeft(width));
		remove.setBounds(bounds.removeFromLeft(width));
		edit.setBounds(bounds.removeFromLeft(width));
	}
private:
	ImageButton add;
	ImageButton remove;
	ImageButton edit;
	Component* contentComponent;
	TableListBox table;
	TableHeaderComponent* header;
	class Model : public TableListBoxModel
	{
		Colour highlightColor;
		ImageButton* buttonToTriggerWhenCellDoubleClicked;
	public:
		void setButtonToTriggerWhenCellDoubleClicked(ImageButton* button) { buttonToTriggerWhenCellDoubleClicked = button; }
		Model(Properties& pProperties)
			: TableListBoxModel(), highlightColor((uint8)135, (uint8)206, (uint8)250, 0.5f), lastUpdate(-1),
			properties(pProperties), buttonToTriggerWhenCellDoubleClicked(nullptr)
		{

		}
		virtual void cellDoubleClicked(int rowNumber, int columnId, const MouseEvent&)
		{
			buttonToTriggerWhenCellDoubleClicked->triggerClick();
		}
		int getLastUpdate() { return lastUpdate; }
		void remove(int i)
		{
			properties.get().erase(properties.get().cbegin() + i);
		}
		std::pair<String, String> getRow(int i) { if (i < getNumRows()) { return properties.get()[i]; } return std::pair<String, String>(); }
		void editValue(int rowNumber, String value) { properties.get()[rowNumber].second = value; }
		bool addRow(std::pair<String, String> property)
		{
			for (int a = 0; a < getNumRows(); a++)
			{
				if (property.first == properties.get()[a].first)
				{
					editValue(a, property.second);
					lastUpdate = a;
					return false;
				}
			}
			properties.add(property);
			lastUpdate = getNumRows() - 1;
			return true;
		}
		void clearRow() { properties.get().clear(); }
		const std::vector<std::pair<String, String>>& getRows() { return properties.get(); }
		virtual int getNumRows() override
		{

			return properties.get().size();
		}
		virtual void paintRowBackground(Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) override
		{
			g.setColour(highlightColor);
			if (rowIsSelected) g.fillRect(0, 0, width, height);
		}
		virtual void paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override
		{
			if (rowNumber < getNumRows())
			{
				g.setColour(Colours::white);
				if (columnId == 1)
				{
					g.drawText(properties.get()[rowNumber].first, 0, 0, width, height, Justification::centredLeft);
				}
				else if (columnId == 2)
				{
					g.drawText(properties.get()[rowNumber].second, 0, 0, width, height, Justification::centredLeft);
				}
			}
		}
	private:

		int lastUpdate;
		Properties& properties;
	};

	Model model;
	Model& getTableModel() { return model; }
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PropertiesWindow)
};