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
/// @file HonokaPlugin.cs
/// @brief HonokaLibネイティブブラグインのラッパクラス 定義
///

using System;
using System.Runtime.InteropServices;


namespace Soundxr.DeviceManager.Honoka.Plugin {

    /// @brief BLE-ID 文字列
    /// @note
    ///  - iOS/macOSの場合 ： BLE-UUID
    using BleId = System.String;

    /// 通知タイプ
    public enum HonokaNotifyType {
        BleState = 1,   ///< BLE状態通知
        DeviceState,    ///< デバイス状態通知
        ErrorCode       ///< エラー通知
    }

    /// BLE状態
    public enum HonokaBleState {
        Unknown = 0,        ///< 状態不明
        Resetting,          ///< システムサービスの接続が一時的に失われ、リセットした
        Unsupported,        ///< このプラットフォームはBLEをサポートしていない
        Unauthorized,       ///< このアプリはBLEの利用許可がない
        PoweredOff,         ///< Bluetoothの設定がOFF.
        PoweredOn           ///< Bluetoothは設定がON.
    }

    /// デバイスの状態
    public enum HonokaDeviceState {
        DidConnect = 1,     ///< 接続した
        DidDisconnect       ///< 切断した
    }
    
    /// エラーコード
    public enum HonokaErrorCode {
        InvalidBleId = 1,   ///< BleIdが不正
        FailedConnect,      ///< 接続に失敗した
        FailedCommand,      ///< コマンドに失敗した
        InvalidCondition,   ///< 状態が不正
        NotImplemented      ///< 未対応
    }


    /// センサタイプ
    [Flags]
    public enum HonokaSensorFlag {
        Invalid = 0,
        Quaternion = 1 << 0,    ///< クォータニオン
        Accelerometer = 1 << 1, ///< 加速度
        Gyroscope = 1 << 2,     ///< ジャイロスコープ
        Compass = 1 << 3        ///< コンパス
    }

    /// センサ取得処理の状態
    public enum HonokaSensorState {
        DidStart = 1,       ///< センサ値取得を開始した
        DidChange,          ///< センサ値を取得した
        DidStop,            ///< センサ値取得を停止した
        FailedToStart = -1, ///< センサ値取得を開始に失敗した
        FailedToStop = -2,  ///< センサ値取得の停止に失敗した
        FailedToRead = -3   ///< センサ値読み取りに失敗した
    }

    /// ノイズモード
    public enum HonokaNoiseMode {
        ANC = 0,    ///< Active Noise Control
        HT,         ///< Hear Through
        Off         ///< なし
    }

    /// ライブラリからの通知を受け取る
    public delegate void HonokaManagerNotifyCallback(HonokaNotifyType type, int param1, string param2);
    /// デバイス検出結果を受け取る
    public delegate void HonokaManagerScanCallback(BleId bleId, string name, int rssi);
    /// デバイスに関する通知を受け取る
    public delegate void HonokaDeviceNotifyCallback(IntPtr objectPtr, HonokaNotifyType type, int param1);
    /// センサ情報を受け取る
    public delegate void HonokaDeviceSensorCallback(IntPtr objectPtr, HonokaSensorState state, HonokaSensorFlag type, float value0, float value1, float value2, float value3);


    /// HonokaLibネイティブブラグインのラッパクラス
    public static class HonokaApi {

        /// HonokaLib の HonokaManager オブジェクトポインタ
        static IntPtr manager;

        /// 初期化する
        /// @param[in] callback 
        public static void Initialize(HonokaManagerNotifyCallback callback) {
            manager = Binding.HonokaManagerNew(callback);
        }
        
        /// 終了処理を行う
        public static void Terminate() {
            Binding.HonokaManagerRelease(manager);
        }

        /// デバイス検出を開始する
        /// @param[in] callback 
        public static void StartScan(HonokaManagerScanCallback callback) {
            Binding.HonokaManagerStartScan(manager, callback);
        }
        
        /// デバイス検出を停止する
        public static void StopScan() {
            Binding.HonokaManagerStopScan(manager);
        }

        /// 指定BLE-IDのHonokaデバイスオブジェクトを取得する
        /// @param[in] bleId 対象HonokaデバイスのBLE-ID
        /// @param[in] objectPtr デバイスと対応させるオブジェクトポインタ
        /// @return デバイスポインタ
        public static IntPtr GetDevice(BleId bleId, IntPtr objectPtr) {
            return Binding.HonokaGet(manager, bleId, objectPtr);
        }

        /// デバイスオブジェクトを解放する
        /// @param[in] device デバイスポインタ
        public static void ReleaseDevice(IntPtr device) {
            Binding.HonokaRelease(device);
        }

        /// 接続する
        /// @param[in] device デバイスポインタ
        /// @param[in] notifyCallback 通知コールバック関数
        /// @param[in] sensorCallback センサ情報コールバック関数
        public static void Connect(IntPtr device, HonokaDeviceNotifyCallback notifyCallback, HonokaDeviceSensorCallback sensorCallback) {
            Binding.HonokaConnect(device, notifyCallback, sensorCallback);
        }

        /// 切断する
        /// @param[in] device デバイスポインタ
        public static void Disconnect(IntPtr device) {
            Binding.HonokaDisconnect(device);
        }

        /// センシングを開始する
        /// @param[in] device デバイスポインタ
        /// @param[in] type センサタイプ
        public static void StartSensing(IntPtr device, HonokaSensorFlag type) {
            Binding.HonokaStartSensing(device, type);
        }

        /// センシングを停止する
        /// @param[in] device デバイスポインタ
        /// @param[in] type センサタイプ
        public static void StopSensing(IntPtr device, HonokaSensorFlag type) {
            Binding.HonokaStopSensing(device, type);
        }
        
        /// モードを設定する
        /// @param[in] device デバイスポインタ
        /// @param[in] mode ノイズモード （ANC／HT／Off）
        public static void SetNoiseMode(IntPtr device, HonokaNoiseMode mode) {
            Binding.HonokaSetNoiseMode(device, mode);
        }
        
        /// 音量を上げる （＋１）
        /// @param[in] device デバイスポインタ
        public static void TurnUpVolume(IntPtr device) {
            Binding.HonokaTurnUpVolume(device);
        }
        
        /// 音量を下げる （−１）
        /// @param[in] device デバイスポインタ
        public static void TurnDownVolume(IntPtr device) {
            Binding.HonokaTurnDownVolume(device);
        }
    }

    /// @brief HonokaLibネイティブライブラリの各メソッドをDllImportするクラス (internal)
    ///
    ///  - HonokaLibネイティブライブラリのC関数を同名のままDllImportして、c＃で利用できるようにしている。
    ///  - internal staticクラス。メンバはinternal static関数。（ Plugin 名前空間内に利用を限定）
    ///  - HonokaApi クラスからのみ利用される。
    internal static class Binding {

#if UNITY_IOS || UNITY_STANDALONE_OSX || UNITY_EDITOR_OSX

#if UNITY_STANDALONE_OSX || UNITY_EDITOR_OSX
        const string LIBNAME = "HonokaLib"; // macOS = HonokaLib.bundle
#else
        const string LIBNAME = "__Internal"; // iOS = HonokaLib.framework
#endif 

        [DllImport(LIBNAME)] internal static extern IntPtr HonokaManagerNew(HonokaManagerNotifyCallback callback);
        [DllImport(LIBNAME)] internal static extern void HonokaManagerRelease(IntPtr manager);
        [DllImport(LIBNAME)] internal static extern void HonokaManagerStartScan(IntPtr manager, HonokaManagerScanCallback callback);
        [DllImport(LIBNAME)] internal static extern void HonokaManagerStopScan(IntPtr manager);
        [DllImport(LIBNAME)] internal static extern IntPtr HonokaGet(IntPtr manager, BleId bleId, IntPtr objectPtr);
        [DllImport(LIBNAME)] internal static extern void HonokaRelease(IntPtr device);
        [DllImport(LIBNAME)] internal static extern void HonokaConnect(IntPtr device, HonokaDeviceNotifyCallback notifyCallback, HonokaDeviceSensorCallback sensorCallback);
        [DllImport(LIBNAME)] internal static extern void HonokaDisconnect(IntPtr device);
        [DllImport(LIBNAME)] internal static extern void HonokaStartSensing(IntPtr device, HonokaSensorFlag type);
        [DllImport(LIBNAME)] internal static extern void HonokaStopSensing(IntPtr device, HonokaSensorFlag type);
        [DllImport(LIBNAME)] internal static extern void HonokaSetNoiseMode(IntPtr device, HonokaNoiseMode mode);
        [DllImport(LIBNAME)] internal static extern void HonokaTurnUpVolume(IntPtr device);
        [DllImport(LIBNAME)] internal static extern void HonokaTurnDownVolume(IntPtr device);

#else
        /// 管理オブジェクトを生成する
        internal static IntPtr HonokaManagerNew(HonokaManagerNotifyCallback callback) { return IntPtr.Zero; }
        /// 管理オブジェクトを破棄する
        internal static void HonokaManagerRelease(IntPtr manager) { }
        /// デバイススキャンを開始する
        internal static void HonokaManagerStartScan(IntPtr manager, HonokaManagerScanCallback callback) { }
        /// デバイススキャンを停止する
        internal static void HonokaManagerStopScan(IntPtr manager) { }
        /// デバイスオブジェクトを取得(生成)する
        internal static IntPtr HonokaGet(IntPtr manager, BleId bleId, IntPtr objectPtr) { return IntPtr.Zero; }
        /// デバイスオブジェクトを解放(破棄)する
        internal static void HonokaRelease(IntPtr device) { }
        /// 接続する
        internal static void HonokaConnect(IntPtr device, HonokaDeviceNotifyCallback notifyCallback, HonokaDeviceSensorCallback sensorCallback) { }
        /// 切断する
        internal static void HonokaDisconnect(IntPtr device) { }
        /// センシングを開始する
        internal static void HonokaStartSensing(IntPtr device, HonokaSensorFlag type) { }
        /// センシングを停止する
        internal static void HonokaStopSensing(IntPtr device, HonokaSensorFlag type) { }
        /// ノイズモードを設定する
        internal static void HonokaSetNoiseMode(IntPtr device, HonokaNoiseMode mode) { }
        /// 音量を上げる
        internal static void HonokaTurnUpVolume(IntPtr device) { }
        /// 音量を下げる
        internal static void HonokaTurnDownVolume(IntPtr device) { }

#endif
    }
}
