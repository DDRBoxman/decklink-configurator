#include <iostream>
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "platform.h"

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	Initialize();

    IDeckLinkIterator *deckLinkIterator;
	GetDeckLinkIterator(&deckLinkIterator);
    IDeckLink *instance;
    while (S_OK == deckLinkIterator->Next(&instance)) {
		STRINGOBJ decklinkName;
		std::string name;

        instance->GetDisplayName(&decklinkName);
		StringToStdString(decklinkName, name);

        std::cout << name << std::endl;

		IDeckLinkConfiguration *deckLinkConfiguration;
		if (instance->QueryInterface(IID_IDeckLink, reinterpret_cast<void**>(&deckLinkConfiguration)) != S_OK) {
			return;
		}

		HRESULT result;
		IDeckLinkAPIInformation* deckLinkInfo;
		if (instance->QueryInterface(IID_IDeckLinkAPIInformation, reinterpret_cast<void**>(&deckLinkInfo)) != S_OK)
		{    std::cout << "Could not find information interface" << std::endl; }

		int64_t id = 0;
		if (deckLinkInfo->GetInt(BMDDeckLinkPersistentID, &id) != S_OK)
		{
			std::cout << "Could not find uuid for decklink device...\n" << std::endl;
		}

		STRINGOBJ newString;

		//StringFromCharArray(&newString, "lol");
		deckLinkConfiguration->GetString(bmdDeckLinkConfigDeviceInformationLabel, &newString);

		deckLinkConfiguration->SetString(bmdDeckLinkConfigDeviceInformationLabel, decklinkName);

		deckLinkConfiguration->WriteConfigurationToPreferences();
    }

}

MainWindow::~MainWindow()
{
}
