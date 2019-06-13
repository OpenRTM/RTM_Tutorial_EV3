// -*- C++ -*-
/*!
 * @file  RobotController.cpp
 * @brief Sample EV3
 * @date $Date$
 *
 * @author 宮本　信彦
 * n-miyamoto@aist.go.jp
 * 産業技術総合研究所　ロボットイノベーション研究センター
 * ロボットソフトウエアプラットフォーム研究チーム
 *
 * LGPL
 *
 * $Id$
 */

#include "RobotController.h"

// Module specification
// <rtc-template block="module_spec">
static const char* robotcontroller_spec[] =
  {
    "implementation_id", "RobotController",
    "type_name",         "RobotController",
    "description",       "Sample EV3",
    "version",           "1.0.0",
    "vendor",            "AIST",
    "category",          "Sample",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.speed_x", "0.0",
    "conf.default.speed_r", "0.0",

    // Widget
    "conf.__widget__.speed_x", "slider.0.01",
    "conf.__widget__.speed_r", "slider.0.01",
    // Constraints
    "conf.__constraints__.speed_x", "-1.0<x<1.0",
    "conf.__constraints__.speed_r", "-2.0<x<2.0",

    "conf.__type__.speed_x", "double",
    "conf.__type__.speed_r", "double",

    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
RobotController::RobotController(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_inIn("in", m_in),
    m_outOut("out", m_out)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
RobotController::~RobotController()
{
}



RTC::ReturnCode_t RobotController::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("in", m_inIn);
  
  // Set OutPort buffer
  addOutPort("out", m_outOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("speed_x", m_speed_x, "0.0");
  bindParameter("speed_r", m_speed_r, "0.0");
  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t RobotController::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RobotController::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RobotController::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t RobotController::onActivated(RTC::UniqueId ec_id)
{
	//センサー値初期化
	for (int i = 0; i < 2; i++)
	{
		sensor_data[i] = false;
	}

	return RTC::RTC_OK;
}


RTC::ReturnCode_t RobotController::onDeactivated(RTC::UniqueId ec_id)
{
	//ロボットを停止する
	m_out.data.vx = 0;
	m_out.data.va = 0;
	m_outOut.write();

	return RTC::RTC_OK;
}


RTC::ReturnCode_t RobotController::onExecute(RTC::UniqueId ec_id)
{
	//入力データの存在確認
	if (m_inIn.isNew())
	{
		//入力データ読み込み
		m_inIn.read();
		for (int i = 0; i < m_in.data.length(); i++)
		{
			//入力データ格納
			if (i < 2)
			{
				sensor_data[i] = m_in.data[i];
			}
		}
	}

	//前進するときのみ停止するかを判定
	if (m_speed_x > 0)
	{
		for (int i = 0; i < 4; i++)
		{
			//タッチセンサのオンオフを判定
			if (sensor_data[i] == true)
			{
				//タッチセンサがオンの場合は停止
				m_out.data.vx = 0;
				m_out.data.va = 0;
				m_outOut.write();
				return RTC::RTC_OK;
			}
		}
	}

	//すべてのタッチセンサがオフの場合はコンフィギュレーションパラメーターの値で操作
	m_out.data.vx = m_speed_x;
	m_out.data.va = m_speed_r;
	m_outOut.write();

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t RobotController::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RobotController::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RobotController::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RobotController::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RobotController::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void RobotControllerInit(RTC::Manager* manager)
  {
    coil::Properties profile(robotcontroller_spec);
    manager->registerFactory(profile,
                             RTC::Create<RobotController>,
                             RTC::Delete<RobotController>);
  }
  
};


