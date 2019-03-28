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
/// @file	HonokaController.h
/// @brief	HonokaControllerクラス 定義
///

#import "HonokaDef.h"

@class HonokaController;

/// Honokaデバイスクラスの通知プロトコル
@protocol HonokaControllerDelegate <NSObject>

@optional

/// デバイスの状態の変化通知
/// @param[in] controller 通知元のデバイスクラス
/// @param[in] state デバイスの状態
- (void)honokaController:(HonokaController *)controller
		  didChangeState:(HonokaState)state;

/// センサデータの通知
/// @param[in] controller 通知元のデバイスクラス
/// @param[in] state 通知のタイプ
/// @param[in] flag センサタイプ
/// @param[in] values センサ値
- (void)honokaController:(HonokaController *)controller
		didChangedSensor:(HonokaSensorState)state
					flag:(HonokaSensorFlag)flag
				  values:(double[])values;

/// エラー通知
/// @param[in] controller 通知元のデバイスクラス
/// @param[in] errorCode エラーコード
- (void)honokaController:(HonokaController *)controller
				 didFail:(HonokaErrorCode)errorCode;

@end

/// Honokaデバイスクラス
@interface HonokaController : NSObject

/// 通知先
@property(weak, nonatomic) id<HonokaControllerDelegate> delegate;

/// デバイスUUID値
@property(strong, nonatomic, readonly) NSUUID *uuid;

/// 接続する
- (void)connect;

/// 切断する
- (void)disconnect;

/// センシングを開始する
/// @param[in] flag センサのタイプ
- (void)startSensing:(HonokaSensorFlag)flag;

/// センシングを停止する
/// @param[in] flag センサのタイプ
- (void)stopSensing:(HonokaSensorFlag)flag;

/// ノイズモードを変更する
/// @param[in] mode ノイズモード(ANC/HT/OFF)
- (void)setNoiseMode:(HonokaNoiseMode)mode;

/// 音量を上げる
- (void)turnUpVolume;

/// 音量を下げる
- (void)turnDownVolume;

@end
