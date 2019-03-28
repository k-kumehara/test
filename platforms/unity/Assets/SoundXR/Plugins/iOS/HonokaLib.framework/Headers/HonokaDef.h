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
/// @file	HonokaDef.h
/// @brief	enum定数 定義
///

/// @defgroup	Enumerations enum
/// @brief	enum定数
/// @{

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/// 通知タイプ
typedef enum {
	HonokaNotifyTypeBleState = 1,	///< BLE状態通知
	HonokaNotifyTypeDeviceState,		///< デバイス状態通知
	HonokaNotifyTypeErrorCode,		///< エラー通知
} HonokaNotifyType;

/// BLE設定の状態
typedef enum {
	HonokaBleStateUnknown = 0,	///< 状態不明
	HonokaBleStateResetting,	///< システムサービスの接続が一時的に失われ、リセットした
	HonokaBleStateUnsupported,	///< このプラットフォームはBLEをサポートしていない
	HonokaBleStateUnauthorized,	///< このアプリはBLEの利用許可がない
	HonokaBleStatePoweredOff,	///< Bluetoothの設定がOFF
	HonokaBleStatePoweredOn,	///< Bluetoothは設定がON
} HonokaBleState;

/// デバイスの状態
typedef enum {
	HonokaStateDidConnect = 1,	///< 接続した
	HonokaStateDidDisconnect	///< 切断した
} HonokaState;

/// エラーコード
typedef enum {
	HonokaErrorCodeInvalidBleId = 1,	///< BleIdが不正
	HonokaErrorCodeFailedConnect,	///< 接続に失敗した
	HonokaErrorCodeFailedCommand,	///< コマンドに失敗した
	HonokaErrorCodeInvalidCondition,	///< 状態が不正
	HonokaErrorCodeNotImplemented,	///< 未対応
} HonokaErrorCode;

/// センサタイプ
typedef enum {
	HonokaSensorFlagInvalid =  0, ///< 無効
	HonokaSensorFlagQuaternion = 1 << 0,	///< クォータニオン
	HonokaSensorFlagAccelerometer = 1 << 1,	///< 加速度
	HonokaSensorFlagGyroscope = 1 << 2,	///< ジャイロ
	HonokaSensorFlagCompass = 1 << 3,	///< コンパス
} HonokaSensorFlag;

/// センサ取得処理の状態
typedef enum {
	HonokaSensorStateDidStart = 1,	///< センサ値取得を開始した
	HonokaSensorStateDidChange,		///< センサ値を取得した
	HonokaSensorStateDidStop,		///< センサ値取得を停止した
	HonokaSensorStateDidFailToStart = -1,	///< センサ値取得を開始に失敗した
	HonokaSensorStateDidFailToStop = -2,	///< センサ値取得の停止に失敗した
	HonokaSensorStateDidFailToRead = -3,	///< センサ値読み取りに失敗した
} HonokaSensorState;

/// ノイズモード
typedef enum {
	HonokaNoiseModeANC = 0,	///< ANC
	HonokaNoiseModeHT,		///< ヒアスルー
	HonokaNoiseModeOff		///< OFF
} HonokaNoiseMode;

#ifdef __cplusplus
}
#endif /* __cplusplus */

/// @}
