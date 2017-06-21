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
#include "Tool.h"
class Modal : public Component, public ButtonListener
{
public:
	enum state
	{
		SUCCESS = 69, CANCEL = 696
	};

	Modal(Vector2i size)
	{
		setSize(size.x, size.y);
		Rectangle<int> bounds = getLocalBounds();
		addAndMakeVisible(success);
		addAndMakeVisible(cancel);
		success.addListener(this);
		cancel.addListener(this);
		success.setButtonText("Yes");
		cancel.setButtonText("No");
		int height = 16;
		int width = getWidth() / 2;
		success.setBounds(0, getHeight() - height, width, height);
		cancel.setBounds(width, getHeight() - height, width, height);
	}
	void buttonClicked(Button* button) override
	{
		if (button == &success)
		{
			if (isSuccess())
			{
				getParentComponent()->exitModalState(SUCCESS);
			}
		}
		getParentComponent()->exitModalState(CANCEL);
		
	}
	virtual bool isSuccess()
	{
		return true;
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
class CreateTabProto : public Modal
{
protected:
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
	CreateTabProto(Vector2i size, int componentHeight)
		: Modal(size)//)
	{

		filename.setText("Name:", NotificationType::dontSendNotification);
		tileWidth.setText("Tile Width:", NotificationType::dontSendNotification);
		tileHeight.setText("Tile Height:", NotificationType::dontSendNotification);

		txtTileHeight.setInputFilter(&filter, false);
		txtTileWidth.setInputFilter(&filter, false);



		int width = size.x / 2;
		int height = componentHeight;

		//Rectangle<int> bounds = getLocalBounds();


		txtTileHeight.setBounds(width, success.getY() - height, width, height);
		tileHeight.setBounds(0, success.getY() - height, width, height);

		txtTileWidth.setBounds(width, txtTileHeight.getY() - height, width, height);
		tileWidth.setBounds(0, txtTileWidth.getY(), width, height);




		filename.setBounds(0, txtTileWidth.getY() - height, width, height);
		txtFilename.setBounds(width, filename.getY(), width, height);

		addAndMakeVisible(filename);
		addAndMakeVisible(txtFilename);
		addAndMakeVisible(tileWidth);
		addAndMakeVisible(txtTileWidth);
		addAndMakeVisible(tileHeight);
		addAndMakeVisible(txtTileHeight);

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
	String getFileName() { return txtFilename.getText(); }
	int getTileWidth()
	{
		return std::stoi(txtTileWidth.getText().toStdString());
	}
	int getTileHeight()
	{
		return std::stoi(txtTileHeight.getText().toStdString());
	}
protected:
	Label filename;
	Label tileHeight;
	Label tileWidth;


	TextEditor txtTileWidth;
	TextEditor txtTileHeight;
	TextEditor txtFilename;
	NumberFilter filter;


};

class CreateEditTab : public CreateTabProto
{
public:
	int getColumnCount()
	{
		return std::stoi(txtTabColumnCount.getText().toStdString());
	}
	int getRowCount()
	{
		return std::stoi(txtTabRowCount.getText().toStdString());
	}
	CreateEditTab()
		: CreateTabProto(Vector2i(300, 16 * 8), 16)
	{
		orientation.setText("Orientation:", NotificationType::dontSendNotification);
		renderOrder.setText("Render Order:", NotificationType::dontSendNotification);

		tabRowCount.setText("Tab Width:", NotificationType::dontSendNotification);
		tabColumnCount.setText("Tab Height:", NotificationType::dontSendNotification);

		int width = 150;
		int height = 16;

		txtTabColumnCount.setInputFilter(&filter, false);
		txtTabRowCount.setInputFilter(&filter, false);



		orientation.setBounds(0, txtFilename.getY() - height, width, height);
		txtOrientation.setBounds(width, orientation.getY(), width, height);

		renderOrder.setBounds(0, orientation.getY() - height, width, height);
		txtRenderOrder.setBounds(width, renderOrder.getY(), width, height);

		txtTabRowCount.setBounds(width, renderOrder.getY() - height, width, height);
		tabRowCount.setBounds(0, txtTabRowCount.getY(), width, height);


		txtTabColumnCount.setBounds(width, txtTabRowCount.getY() - height, width, height);
		tabColumnCount.setBounds(0, txtTabColumnCount.getY(), width, height);


		addAndMakeVisible(tabRowCount);
		addAndMakeVisible(txtTabColumnCount);
		addAndMakeVisible(tabColumnCount);
		addAndMakeVisible(txtTabRowCount);

		addAndMakeVisible(orientation);
		addAndMakeVisible(txtOrientation);
		addAndMakeVisible(renderOrder);
		addAndMakeVisible(txtRenderOrder);

	}
	String getOrientation() { return txtOrientation.getText(); }
	String getRenderOrder() { return txtRenderOrder.getText(); }
private:
	Label tabRowCount;
	Label tabColumnCount;
	Label orientation;
	Label renderOrder;

	TextEditor txtTabColumnCount;
	TextEditor txtTabRowCount;
	TextEditor txtRenderOrder;
	TextEditor txtOrientation;
};

class CreateTilesetTab : public CreateTabProto
{
public:
	CreateTilesetTab()
		: CreateTabProto(Vector2i(300 + 32, 16 * 7), 16)
	{
		image.setText("Image file:", NotificationType::dontSendNotification);
		spacing.setText("Spacing:", NotificationType::dontSendNotification);
		margin.setText("Margin:", NotificationType::dontSendNotification);
		

		Colour tWhite((uint8)0, (uint8)0, (uint8)0, 0.5f);
		Colour tGrey((uint8)123, (uint8)123, (uint8)123, 0.5f);
		browseImage.setImages(false, true, true, getImageFromFile("Asset/fileopen.png"), 1.0f, Colour(), Image(), 1.0f, tWhite, Image(), 1.0f, tGrey);

		int width = 166;
		int height = 16;
		int buttonWidth = 32;
		
		spacing.setBounds(0, txtFilename.getY() - height, width, height);
		txtSpacing.setBounds(width, spacing.getY(), width, height);


		margin.setBounds(0, spacing.getY() - height, width, height);
		txtMargin.setBounds(width, margin.getY(), width, height);

		image.setBounds(0, margin.getY() - height, width, height);
		txtImage.setBounds(width, image.getY(), width - buttonWidth, height);

		browseImage.setBounds(txtImage.getX() + txtImage.getWidth(), txtImage.getY(), buttonWidth, height);

		txtSpacing.setInputFilter(&filter, false);
		txtMargin.setInputFilter(&filter, false);
		 

		browseImage.addListener(this);

		addAndMakeVisible(image);
		addAndMakeVisible(txtImage);
		addAndMakeVisible(browseImage);
		addAndMakeVisible(txtSpacing);
		addAndMakeVisible(txtMargin);
		addAndMakeVisible(image);
		addAndMakeVisible(spacing);
		addAndMakeVisible(margin);

	}
	void buttonClicked(Button* button) override
	{

		if (button == &browseImage)
		{
			FileChooser file("Open image", File(), "*.png, *.jpeg", false);

			if (file.browseForFileToOpen())
			{
				File result = file.getResult();
				txtImage.setText(result.getFullPathName(), NotificationType::dontSendNotification);
				txtFilename.setText(result.getFileNameWithoutExtension(), NotificationType::dontSendNotification);
			
			}
		}
		else
			Modal::buttonClicked(button);
	}
	bool isSuccess() override
	{
		if (!File(txtImage.getText()).exists())
		{
			WarningModal warning(txtImage.getText() + " doesnt exist!", Vector2i(150, 150));
			openModal(warning);
			return false;
		}
		
		return true;
	}
	File getFile()
	{
		return File(txtImage.getText());
	}
	int getSpacing()
	{
		return std::stoi(txtSpacing.getText().toStdString());
	}
	int getMargin()
	{
		return std::stoi(txtMargin.getText().toStdString());
	}
private:
	Label image;
	Label spacing;
	Label margin;
	TextEditor txtImage;
	TextEditor txtMargin;
	TextEditor txtSpacing;
	ToolButton browseImage;
};







