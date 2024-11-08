// qt-install-script.qs
function Controller() {
    installer.autoAcceptLicenses();
    installer.setVerbose(true);
}

Controller.prototype.ComponentSelectionPageCallback = function() {
    var widget = gui.pageWidgetByObjectName("ComponentSelectionPage");
    var qtComponent = "qt.qt6.650.win64_msvc2019_64";

    function selectComponent(name) {
        var component = installer.componentByName(name);
        if (component) {
            component.selected = true;
        }
    }

    selectComponent(qtComponent);
    widget.clickButton("NextButton");
};

Controller.prototype.IntroductionPageCallback = function() {
    gui.clickButton("NextButton");
};

Controller.prototype.TargetDirectoryPageCallback = function() {
    var widget = gui.pageWidgetByObjectName("TargetDirectoryPage");
    widget.TargetDirectoryLineEdit.setText("C:\\Qt");
    widget.clickButton("NextButton");
};

Controller.prototype.LicenseAgreementPageCallback = function() {
    gui.clickButton("NextButton");
};

Controller.prototype.ReadyForInstallationPageCallback = function() {
    gui.clickButton("NextButton");
};

Controller.prototype.FinishedPageCallback = function() {
    gui.clickButton("FinishButton");
};
