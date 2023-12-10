
#ifndef Q_MOC_RUN
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>
#include <ros/ros.h>
#include <ros/package.h>
#include <yaml-cpp/yaml.h>
#endif

#include <rviz/panel.h>

namespace Ui
{
class PluginUI;
}

namespace rosbag_play_rviz_plugin
{
class RosbagPlayPlugin : public rviz::Panel
{
  Q_OBJECT
public:
  RosbagPlayPlugin(QWidget* parent = nullptr);
  ~RosbagPlayPlugin() override;

  void onInitialize() override;
  void onEnable();
  void onDisable();

private Q_SLOTS:
  void pb_bagClicked();
  void pb_startClicked();
  void pb_stopClicked();

private:
  ros::NodeHandle nh_;
  std::string player_name_;

protected:
  Ui::PluginUI* ui_;
};
}
