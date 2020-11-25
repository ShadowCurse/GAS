#include "gui_core.hpp"

GuiCore::GuiCore() : QObject() {
    connect(&gui, &GasGui::signal_first_connect, this,
            &GuiCore::slot_first_connect);
    connect(&gui, &GasGui::signal_first_login, this,
            &GuiCore::slot_first_login);
    connect(&gui, &GasGui::signal_first_create_user, this,
            &GuiCore::slot_first_create_user);
    connect(&gui, &GasGui::signal_connector_state_change, this,
            &GuiCore::slot_connector_state_change);
    connect(&gui, &GasGui::signal_create_resource_tree, this,
            &GuiCore::slot_create_resource_tree);
    connect(&gui, &GasGui::signal_create_commits_list, this,
            &GuiCore::slot_create_commits_list);
    connect(&gui, &GasGui::signal_create_users_list, this,
            &GuiCore::slot_create_users_list);
    connect(&gui, &GasGui::signal_create_logs_list, this,
            &GuiCore::slot_create_logs_list);
    connect(&gui, &GasGui::signal_get_resource_info, this,
            &GuiCore::slot_send_resource_info);
    connect(&gui, &GasGui::signal_get_commit_info, this,
            &GuiCore::slot_send_commit_info);
    connect(&gui, &GasGui::signal_get_user_info, this,
            &GuiCore::slot_send_user_info);
    connect(&gui, &GasGui::signal_add_new_resource, this,
            &GuiCore::slot_add_new_resource);
    connect(&gui, &GasGui::signal_add_new_resource, this,
            &GuiCore::slot_add_new_resource);
    connect(&gui, &GasGui::signal_update_resource, this,
            &GuiCore::slot_update_resource);
    connect(&gui, &GasGui::signal_download_resource, this,
            &GuiCore::slot_download_resource);
    connect(&gui, &GasGui::signal_remove_resource, this,
            &GuiCore::slot_remove_resource);
    connect(&gui, &GasGui::signal_request_connector_settings, this,
            &GuiCore::slot_send_connector_settings);
    connect(&gui, &GasGui::signal_request_connector_create, this,
            &GuiCore::slot_create_connector);
    connect(&gui, &GasGui::signal_request_connector_remove, this,
            &GuiCore::slot_remove_connector);

    connect(this, &GuiCore::signal_starting_connection_state_change, &gui,
            &GasGui::slot_starting_connection_state_change);
    connect(this, &GuiCore::signal_accept_user, &gui,
            &GasGui::slot_accept_user);
    connect(this, &GuiCore::signal_deny_user, &gui,
            &GasGui::slot_deny_user);
    connect(this, &GuiCore::signal_draw_resource_tree, &gui,
            &GasGui::slot_draw_resource_tree);
    connect(this, &GuiCore::signal_draw_commits_list, &gui,
            &GasGui::slot_draw_commits_list);
    connect(this, &GuiCore::signal_draw_users_list, &gui,
            &GasGui::slot_draw_users_list);
    connect(this, &GuiCore::signal_draw_logs_list, &gui,
            &GasGui::slot_draw_logs_list);
    connect(this, &GuiCore::signal_send_connector_settings, &gui,
            &GasGui::slot_show_connector_settings);
    connect(this, &GuiCore::signal_create_connector, &gui,
            &GasGui::slot_create_connector);
    connect(this, &GuiCore::signal_remove_connector, &gui,
            &GasGui::slot_remove_connector);
    connect(this, &GuiCore::signal_log_message, &gui,
            &GasGui::log_message);
    connect(this, &GuiCore::signal_log_warning, &gui,
            &GasGui::log_warning);
    connect(this, &GuiCore::signal_log_error, &gui,
            &GasGui::log_error);
}

void GuiCore::slot_first_connect(GuiCore::connector_info info)
{
    emit signal_starting_connection_state_change(true);
    info.id = 0;
    emit signal_create_connector(std::move(info));
}

void GuiCore::slot_first_login(GuiCore::login_info)
{
    emit signal_accept_user();
}

void GuiCore::slot_first_create_user(GuiCore::create_user_info)
{

}

void GuiCore::slot_connector_state_change(GuiCore::id_type, bool)
{

}

void GuiCore::slot_create_resource_tree()
{

}

void GuiCore::slot_create_commits_list()
{

}

void GuiCore::slot_create_users_list()
{

}

void GuiCore::slot_create_logs_list()
{

}

void GuiCore::slot_send_resource_info(GuiCore::id_type)
{

}

void GuiCore::slot_send_commit_info(GuiCore::id_type)
{

}

void GuiCore::slot_send_user_info(GuiCore::id_type)
{

}

void GuiCore::slot_add_new_resource(GuiCore::resource_create_type)
{

}

void GuiCore::slot_update_resource(GuiCore::resource_update_type)
{

}

void GuiCore::slot_download_resource(GuiCore::resource_download_type)
{

}

void GuiCore::slot_remove_resource(GuiCore::resource_delete_type)
{

}

void GuiCore::slot_send_connector_settings(GuiCore::id_type)
{

}

void GuiCore::slot_create_connector(GuiCore::connector_info)
{

}

void GuiCore::slot_remove_connector(GuiCore::id_type)
{

}
