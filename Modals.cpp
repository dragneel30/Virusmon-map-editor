
#include "../JuceLibraryCode/JuceHeader.h"
#include "Modals.h"

int openModal(Modal& pModal)
{
	DialogWindow::LaunchOptions modal;
	modal.dialogTitle = "modal";
	modal.dialogBackgroundColour = Colours::darkgrey;
	modal.content = OptionalScopedPointer<Component>(&pModal, false);
	modal.componentToCentreAround = nullptr;
	modal.escapeKeyTriggersCloseButton = true;
	modal.useNativeTitleBar = true;
	modal.resizable = false;
	modal.useBottomRightCornerResizer = false;
	return modal.runModal();
}