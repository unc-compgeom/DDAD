void MainWindow::on_create_polyline_toggled(bool checked) {
    qDebug() << "on_create_polyline_toggled: " << checked;

    static PolylineCreationMethod *creation_method = nullptr;

    if (checked) {
        ConfigManager::get().set_input_state(InputState::CREATE_POLYLINE);
        uncheckInputModeButtons();

        creation_method = new PolylineCreationMethod();

        QLayoutItem *spacer = ui->create_tab_spacer;
        ui->create->layout()->removeItem(spacer);
        ui->create->layout()->addWidget(creation_method);
        ui->create->layout()->addItem(spacer);

    } else if (creation_method) {
        ui->create->layout()->removeWidget(creation_method);
        delete creation_method;
        creation_method = nullptr;
    }
}