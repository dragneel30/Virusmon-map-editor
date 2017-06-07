/*
  ==============================================================================

    Modals.h
    Created: 25 May 2017 5:04:14pm
    Author:  Lorence

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "Vector2.h"

class Modal : public Component, public ButtonListener
{
public:
	enum state
	{
		SUCCESS = 69, CANCEL = 696
	};

	Modal(Vector2i size)
	{
		setSize(300, 300);
		Rectangle<int> bounds = getLocalBounds();
		addAndMakeVisible(success);
		addAndMakeVisible(cancel);
		success.addListener(this);
		cancel.addListener(this);
		success.setButtonText("Yes");
		cancel.setButtonText("No");
		int height = 32;
		int halfWidth = getWidth() / 2;
		success.setBounds(0, getHeight() - height, halfWidth, height);
		cancel.setBounds(halfWidth, getHeight() - height, halfWidth, height);
	}
	void buttonClicked(Button* button) override
	{
		if (button == &success)
		{
			getParentComponent()->exitModalState(SUCCESS);
		}
		else if (button == &cancel)
		{
			getParentComponent()->exitModalState(CANCEL);
		}
	}
protected:
	TextButton success;
	TextButton cancel;
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Modal)
};


int openModal(Modal& pModal);
class WarningModal : public Modal
{

public:
	WarningModal(String message, Vector2i size)
		: Modal(size)
	{
		addAndMakeVisible(label);
		label.setText(message, NotificationType::dontSendNotification);
		label.setBounds(0, 0, 50, 50);
	//	label.setBounds(size.x / 2 - (label.getWidth() / 2), (size.y - success.getHeight()) / 2 - (label.getHeight() / 2), label.getWidth(), label.getHeight());
	}
private:
	Label label;
};
#include "Util.h"
class CreateModal : public Modal
{
	class NumberFilter : public TextEditor::InputFilter
	{
	public:

		virtual String filterNewText(TextEditor& editor, const String& newInput)
		{
			for (int a = 0; a < newInput.length(); a++)
			{
				if (newInput[a] - 48 > 9 || newInput[a] - 48 < 0) // char - 48 should be from 0 to 9
				{
					return "";
				}
			}
			return newInput;
		}
	};
public:
	CreateModal()
		: Modal(Vector2i(100,300 + 32))
	{

		filename.setText("Map name:", NotificationType::dontSendNotification);
		mapWidth.setText("Map Width:", NotificationType::dontSendNotification);
		mapHeight.setText("Map Height:", NotificationType::dontSendNotification);
		orientation.setText("Orientation:", NotificationType::dontSendNotification);
		renderOrder.setText("Render order:", NotificationType::dontSendNotification);
		tileWidth.setText("Tile Width:", NotificationType::dontSendNotification);
		tileHeight.setText("Tile Width:", NotificationType::dontSendNotification);
		renderOrderVal.setText("Bottom right", NotificationType::dontSendNotification);
		orientationVal.setText("Orthogonal", NotificationType::dontSendNotification);

		txtTileHeight.setInputFilter(&filter, false);
		txtTileWidth.setInputFilter(&filter, false);
		txtMapWidth.setInputFilter(&filter, false);
		txtMapHeight.setInputFilter(&filter, false);

		txtFilename.setText("default");
		
		
		int width = (getWidth())/2;
		int height = (getHeight() - 32)/7;

		Rectangle<int> bounds = getLocalBounds();

		txtTileHeight.setBounds(width, success.getY() - height, width, height);
		tileHeight.setBounds(0, success.getY() - height, width, height);

		txtTileWidth.setBounds(width, txtTileHeight.getY() - height, width, height);
		tileWidth.setBounds(0, txtTileWidth.getY(), width, height);


		txtMapHeight.setBounds(width, txtTileWidth.getY() - height, width, height);
		mapHeight.setBounds(0, txtMapHeight.getY(), width, height);


		txtMapWidth.setBounds(width, txtMapHeight.getY() - height, width, height);
		mapWidth.setBounds(0, txtMapWidth.getY(), width, height);

		renderOrderVal.setBounds(width, txtMapWidth.getY() - height, width, height);
		renderOrder.setBounds(0, renderOrderVal.getY(), width, height);

		orientationVal.setBounds(width, renderOrderVal.getY() - height, width, height);
		orientation.setBounds(0, orientationVal.getY(), width, height);

		filename.setBounds(0, orientationVal.getY() - height, width, height);
		txtFilename.setBounds(width, filename.getY(), width, height);

		addAndMakeVisible(filename);
		addAndMakeVisible(txtFilename);
		addAndMakeVisible(orientation);
		addAndMakeVisible(orientationVal);
		addAndMakeVisible(renderOrder);
		addAndMakeVisible(renderOrderVal);
		addAndMakeVisible(mapWidth);
		addAndMakeVisible(txtMapWidth);
		addAndMakeVisible(mapHeight);
		addAndMakeVisible(txtMapHeight);
		addAndMakeVisible(tileWidth);
		addAndMakeVisible(txtTileWidth);
		addAndMakeVisible(tileHeight);
		addAndMakeVisible(txtTileHeight);

	}
    String getFileName()
	{
		return txtFilename.getText();
	}
	Vector2i getTileSize()
	{
		return Vector2i(std::stoi(txtTileWidth.getText().toStdString()), std::stoi(txtTileHeight.getText().toStdString()));
	}
	Vector2i getMapSize()
	{
		return Vector2i(std::stoi(txtMapWidth.getText().toStdString()), std::stoi(txtMapHeight.getText().toStdString()));
	}
	void buttonClicked(Button* button) override
	{
		if (button == &success)
		{
			if (getFileName().isEmpty())
			{
				WarningModal warning("Map name cannot be empty!", Vector2i(300, 200));
				openModal(warning);
			}
			else
				getParentComponent()->exitModalState(SUCCESS);
		}
		else if (button == &cancel)
		{
			getParentComponent()->exitModalState(CANCEL);
		}
	}
private:
	Label filename;
	Label mapWidth;
	Label mapHeight;
	Label orientation;
	Label renderOrder;
	Label tileWidth;
	Label tileHeight;

	Label renderOrderVal;
	Label orientationVal;

	TextEditor txtTileWidth;
	TextEditor txtTileHeight;
	TextEditor txtMapWidth;
	TextEditor txtMapHeight;
	TextEditor txtFilename;
	NumberFilter filter;
};
