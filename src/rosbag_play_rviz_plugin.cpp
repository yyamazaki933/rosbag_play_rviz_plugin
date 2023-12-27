#include <rosbag_play_rviz_plugin.h>

#include <pluginlib/class_list_macros.h>
#include <Qt>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QFileDialog>
#include "ui_rosbag_play_rviz_plugin.h"
#include "ros/ros.h"

namespace rosbag_play_rviz_plugin
{
RosbagPlayPlugin::RosbagPlayPlugin(QWidget* parent) : Panel(parent), ui_(new Ui::PluginUI())
{
  ui_->setupUi(this);
}

RosbagPlayPlugin::~RosbagPlayPlugin() = default;

void RosbagPlayPlugin::onInitialize()
{
  connect(ui_->pb_bags, SIGNAL(clicked()), this, SLOT(pb_bagClicked()));
  connect(ui_->pb_start, SIGNAL(clicked()), this, SLOT(pb_startClicked()));
  connect(ui_->pb_stop, SIGNAL(clicked()), this, SLOT(pb_stopClicked()));

  ui_->pb_start->setDisabled(false);
  ui_->pb_stop->setDisabled(true);

  parentWidget()->setVisible(true);
}

void RosbagPlayPlugin::onEnable()
{
  show();
  parentWidget()->show();
}

void RosbagPlayPlugin::onDisable()
{
  hide();
  parentWidget()->hide();
}

void RosbagPlayPlugin::pb_bagClicked()
{
  QStringList files = QFileDialog::getOpenFileNames(this, "Open Rosbags", "/home", "Rosbag (*.bag)");
  ui_->le_bags->setText(files.join(" "));
}

void RosbagPlayPlugin::pb_startClicked()
{
  if (ui_->pb_start->text() == "Pause")
  {
    std::string command = "rosservice call " + player_name_ + "/pause_playback true";
    ROS_INFO_STREAM(command);
    system(command.c_str());
    ui_->pb_start->setText("Resume");
    return;
  }
  else if (ui_->pb_start->text() == "Resume")
  {
    std::string command = "rosservice call " + player_name_ + "/pause_playback false";
    ROS_INFO_STREAM(command);
    system(command.c_str());
    ui_->pb_start->setText("Pause");
    return;
  }

  std::string bags = ui_->le_bags->text().toStdString();
  double rate = ui_->dsb_rate->value();
  int offset = ui_->sb_ofst->value();

  player_name_ = "player_plugin_" + std::to_string(ros::Time::now().sec);

  std::string command = "rosbag play __name:=" + player_name_ + " " + bags + " --clock";
  if (rate != 1.0)
    command += " --rate=" + std::to_string(rate);
  if (offset != 0)
    command += " --start=" + std::to_string(offset);
  command += " &";
  ROS_INFO_STREAM(command);
  system(command.c_str());

  ui_->pb_start->setText("Pause");
  ui_->pb_stop->setDisabled(false);
}

void RosbagPlayPlugin::pb_stopClicked()
{
  std::string command = "rosnode kill " + player_name_;
  ROS_INFO_STREAM(command);
  system(command.c_str());

  ui_->pb_start->setText("Start");
  ui_->pb_stop->setDisabled(true);
}

}
PLUGINLIB_EXPORT_CLASS(rosbag_play_rviz_plugin::RosbagPlayPlugin, rviz::Panel)
