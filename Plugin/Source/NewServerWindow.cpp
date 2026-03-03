/*
 * Copyright (c) 2020 Andreas Pohl
 * Licensed under MIT (https://github.com/apohl79/audiogridder/blob/master/COPYING)
 *
 * Author: Andreas Pohl
 */

#include "NewServerWindow.hpp"

#include <JuceHeader.h>

namespace e47 {

NewServerWindow::NewServerWindow(float x, float y) : TopLevelWindow("New Server", true) {
    setBounds((int)lroundf(x), (int)lroundf(y), 320, 95);

    m_serverLabel.setText("Host (IP or hostname, optional: hostname:0:DisplayName)", NotificationType::dontSendNotification);
    m_serverLabel.setJustificationType(Justification::topLeft);
    addChildAndSetID(&m_serverLabel, "serverLabel");
    m_serverLabel.setBounds(5, 2, 310, 18);

    addChildAndSetID(&m_server, "server");
    m_server.setBounds(5, 20, 310, 25);
    m_server.setPlaceholder("e.g. 192.168.1.10 or myhost.dyndns.org:0:Remote");

    addChildAndSetID(&m_cancel, "cancel");
    m_cancel.setBounds(5, 52, 90, 25);
    m_cancel.setButtonText("Cancel");
    m_cancel.addListener(this);

    addChildAndSetID(&m_ok, "ok");
    m_ok.setBounds(215, 52, 100, 25);
    m_ok.setButtonText("Add");
    m_ok.addListener(this);

    setVisible(true);
}

NewServerWindow::~NewServerWindow() {}

void NewServerWindow::paint(Graphics& g) {
    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));  // clear the background
}

void NewServerWindow::buttonClicked(Button* button) {
    if (button->getButtonText() == "Add" && m_onOk != nullptr) {
        m_onOk(m_server.getTextValue().toString());
    }
    delete this;
}

void NewServerWindow::activeWindowStatusChanged() {
    TopLevelWindow::activeWindowStatusChanged();
    if (!isActiveWindow()) {
        delete this;
    }
}

}  // namespace e47
