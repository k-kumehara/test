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
/// @file   HonokaManager.h
/// @brief  HonokaManagerクラス 定義
///

#import "HonokaDef.h"

@class HonokaManager;
@class HonokaController;

/// Honoka管理クラスの通知プロトコル
@protocol HonokaManagerDelegate <NSObject>

/// BLEの状態変化の通知
/// @param[in] manager 通知元
/// @param[in] state BLEの状態
- (void)honokaManager:(HonokaManager *)manager didChangeBleState:(HonokaBleState)state;

/// デバイスの状態変化の通知
/// @param[in] manager 通知元
/// @param[in] state デバイスの状態
/// @param[in] uuid デバイスID
- (void)honokaManager:(HonokaManager *)manager didChangeConnection:(HonokaState)state uuid:(NSUUID *)uuid;

/// エラー通知
/// @param[in] manager 通知元
/// @param[in] errorCode エラーコード
/// @param[in] uuid デバイスID
- (void)honokaManager:(HonokaManager *)manager didFailed:(HonokaErrorCode)errorCode uuid:(NSUUID *)uuid;

/// Honokaデバイス検出の通知
/// @param[in] manager 通知元
/// @param[in] uuid デバイスID
/// @param[in] name デバイス名称
/// @param[in] rssi 受信信号強度 RSSI
- (void)honokaManager:(HonokaManager *)manager didDiscoverHonoka:(NSUUID *)uuid name:(NSString *)name rssi:(int)rssi;

@end

/// Honoka管理クラス
@interface HonokaManager : NSObject

/// 通知先
@property(weak, nonatomic) id<HonokaManagerDelegate> delegate;

/// BLEの状態
@property(assign, nonatomic, readonly) HonokaBleState state;

/// Honokaデバイスのスキャン中か否か
@property(assign, nonatomic, readonly) bool isScanning;

/// スキャンを開始する
- (bool)startScan;

/// スキャンを停止する
- (void)stopScan;

/// Honokaデバイスクラスを取得する
/// @param[in] uuid デバイスのUUID値
- (HonokaController *)getController:(NSUUID *)uuid;

@end
