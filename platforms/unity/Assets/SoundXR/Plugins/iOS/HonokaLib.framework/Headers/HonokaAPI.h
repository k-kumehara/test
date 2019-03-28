/*!
 * Copyright 2019 Yamaha Corp. All Rights Reserved.
 *
 * The content of this file includes portions of the Yamaha Sound xR
 * released in source code form as part of the SDK package.
 *
 * Commercial License Usage
 *
 * Licensees holding valid commercial licenses to the Yamaha Sound xR
 * may use this file in accordance with the end user license agreement
 * provided with the software or, alternatively, in accordance with the
 * terms contained in a written agreement between you and Yamaha Corp.
 *
 * Apache License Usage
 *
 * Alternatively, this file may be used under the Apache License, Version 2.0 (the "Apache License");
 * you may not use this file except in compliance with the Apache License.
 * You may obtain a copy of the Apache License at
 * http://www.apache.org/licenses/LICENSE-2.0.
 *
 * Unless required by applicable law or agreed to in writing, software distributed
 * under the Apache License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES
 * OR CONDITIONS OF ANY KIND, either express or implied. See the Apache License for
 * the specific language governing permissions and limitations under the License.
 */

///
/// @file	HonokaApi.h
/// @brief	C-API 定義
///

#import "HonokaDef.h"

/// @defgroup	CAPI C-API
/// @brief	CーAPI for Unity
/// @{

//
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
	
	typedef const char *HonokaBleId;	///< HonokaデバイスのBLE UUID
	typedef const void *HonokaManagerPtr;	///< 管理オブジェクトのポインタ
	typedef const void *HonokaControllerPtr;	///< デバイスオブジェクトのポインタ
	typedef const void *ObjectPtr;	///< UnityのGameObjectポインタ

	//--------------------------------------------------------------------------
	
	/// ライブラリからの通知を受け取る
	/// @param[in] type 通知タイプ
	/// @param[in] param1 通知情報1
	/// @param[in] param2 通知情報2
	typedef void (*HonokaManagerNotifyCallback)(HonokaNotifyType type, int param1, const char *param2);
	
	/// スキャン結果を受け取る
	/// @param[in] bleId 検出したHonokaデバイスのBLE UUID
	/// @param[in] name 検出したHonokaデバイスの名前
	/// @param[in] rssi 受信信号強度 RSSI
	typedef void (*HonokaManagerScanCallback)(HonokaBleId bleId, const char *name, int rssi);
	
	/// デバイスに関する通知を受け取る
	/// @param[in] object 通知先UnityObject
	/// @param[in] type 通知タイプ
	/// @param[in] param 通知情報
	typedef void (*HonokaDeviceNotifyCallback)(ObjectPtr object, HonokaNotifyType type, int param);
	
	/// センサ情報を受け取る
	/// @param[in] object 通知先UnityObject
	/// @param[in] state 通知データタイプ
	/// @param[in] flag センサタイプ
	/// @param[in] value0 センサ値1
	/// @param[in] value1 センサ値2
	/// @param[in] value2 センサ値3
	/// @param[in] value3 センサ値4
	typedef void (*HonokaDeviceSensorCallback)(ObjectPtr object, HonokaSensorState state, HonokaSensorFlag flag, float value0, float value1, float value2, float value3);
	
	//--------------------------------------------------------------------------
	
	/// HonokaManagerを生成する
	/// @param[in] callback 通知コールバック関数ポインタ
	/// @return  管理オブジェクトのポインタ
	HonokaManagerPtr HonokaManagerNew(HonokaManagerNotifyCallback callback);
	
	/// HonokaManagerを破棄する
	/// @param[in] manager 管理オブジェクトのポインタ
	void HonokaManagerRelease(HonokaManagerPtr manager);
	
	/// Honokaデバイス検出を開始する
	/// @param[in] manager 管理オブジェクトのポインタ
	/// @param[in] callback 検出コールバック関数ポインタ
	void HonokaManagerStartScan(HonokaManagerPtr manager, HonokaManagerScanCallback callback);
	
	/// Honokaデバイス検出を停止する
	/// @param[in] manager 管理オブジェクトのポインタ
	void HonokaManagerStopScan(HonokaManagerPtr manager);
	
	/// Honokaデバイスを取得する
	/// @param[in] manager 管理オブジェクトのポインタ
	/// @param[in] bleId 対象デバイスのID
	/// @param[in] object 対応するUnityのゲームオブジェクト
	/// @return  デバイスオブジェクトのポインタ
	HonokaControllerPtr HonokaGet(HonokaManagerPtr manager, HonokaBleId bleId, ObjectPtr object);
	
	/// Honokaデバイスを解放する
	/// @param[in] controller 対象デバイスオブジェクトのポインタ
	void HonokaRelease(HonokaControllerPtr controller);
	
	/// Honokaデバイスと接続する
	/// @param[in] controller 対象デバイスオブジェクトのポインタ
	/// @param[in] notifyCallback 通知コールバック関数ポインタ
	/// @param[in] sensorCallback センサ情報コールバック関数ポインタ
	void HonokaConnect(HonokaControllerPtr controller, HonokaDeviceNotifyCallback notifyCallback, HonokaDeviceSensorCallback sensorCallback);
	
	/// Honokaデバイスとの接続を切る
	/// @param[in] controller 対象デバイスオブジェクトのポインタ
	void HonokaDisconnect(HonokaControllerPtr controller);
	
	/// センシングを開始する
	/// @param[in] controller 対象デバイスオブジェクトのポインタ
	/// @param[in] type 開始するセンサタイプ
	void HonokaStartSensing(HonokaControllerPtr controller, HonokaSensorFlag type);
	
	/// センシングを停止する
	/// @param[in] controller 対象デバイスオブジェクトのポインタ
	/// @param[in] type 開始するセンサタイプ
	void HonokaStopSensing(HonokaControllerPtr controller, HonokaSensorFlag type);
	
	/// ノイズモードを変更する
	/// @param[in] controller 対象デバイスオブジェクトのポインタ
	/// @param[in] mode 設定するモード
	void HonokaSetNoiseMode(HonokaControllerPtr controller, HonokaNoiseMode mode);
	
	/// 音量を上げる
	void HonokaTurnUpVolume(HonokaControllerPtr controller);

	/// 音量を下げる
	void HonokaTurnDownVolume(HonokaControllerPtr controller);

#ifdef __cplusplus
}
#endif // __cplusplus

/// @}

