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
/// @file HonokaDeviceManager.cs
/// @brief HonokaDeviceManager クラス 定義
///

using System;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Assertions;
using AOT;
using Soundxr.DeviceManager.Honoka.Plugin;


namespace Soundxr.DeviceManager.Honoka {

    /// Honokaデバイス管理クラス
    [AddComponentMenu("Sound xR/HonokaDeviceManager")]
    public class HonokaDeviceManager : MonoBehaviour {

        /// 唯一のインスタンス
        static HonokaDeviceManager _instance;

        /// 唯一のインスタンス (シーン上にいる本クラスのインスタンスを探す、存在しなけばnull)
        public static HonokaDeviceManager instance {
            get {
                if (_instance == null) {
                    _instance = FindObjectOfType<HonokaDeviceManager>();
                    Assert.IsNotNull(_instance, typeof(HonokaDeviceManager) + " is nothing.");
                }
                return _instance;
            }
        }

        /// インスタンスがロードされた時に呼び出される。１個目ならば初期化し、２個目ならば破棄する
        void Awake() {
            if (this != instance) {
                Destroy(this);
                return;
            }
            DontDestroyOnLoad(this.gameObject);
            Initialize();
        }

        /// アプリ終了時に呼び出される。終了処理を行う
        void OnApplicationQuit() {
            Terminate();
        }

        //-------------------------------------------------------------------------

        /// BLEの状態
        public HonokaBleState bleState { get; private set; }
        /// 検出したHonokaデバイス情報のリスト
        public List<DeviceInfo> devices { get; private set; }
        /// 検出中か否か
        public bool scanning { get; private set; }
        /// 初期化済みか否か
        private bool initialized = false;

        //-------------------------------------------------------------------------

        /// BLE状態変更イベント
        public event Action<HonokaBleState> BleStateEvent;
        /// デバイス状態変更イベント
        public event Action<HonokaDeviceState, string> DeviceStateEvent;
        /// エラーイベント
        public event Action<HonokaErrorCode> ErrorEvent;
        /// デバイス検出イベント
        public event Action<String, String, int> ScanEvent;

        //-------------------------------------------------------------------------

        /// 初期化する
        void Initialize() {
            if (initialized) {
                return;
            }
            devices = new List<DeviceInfo>();
            HonokaApi.Initialize(OnNotify);
            initialized = true;
        }

        /// 終了する
        void Terminate() {
            if (!initialized) {
                return;
            }
            HonokaApi.Terminate();
            initialized = false;
        }

        /// Honokaデバイスの検出を開始する
        public void StartScan() {
            Assert.IsTrue(initialized, "Not initialized.");
            devices = new List<DeviceInfo>();
            scanning = true;
            HonokaApi.StartScan(OnScan);
        }

        /// Honokaデバイスの検出を停止する
        public void StopScan() {
            Assert.IsTrue(initialized, "Not initialized.");
            if (scanning) {
                HonokaApi.StopScan();
                scanning = false;
            }
        }

        //-------------------------------------------------------------------------

        /// BLE状態変更通知を受け取る
        /// @param[in] state BLEの状態
        internal void OnNotifyBleState(HonokaBleState state) {
            Debug.LogFormat("OnNotifyBleState/ state:{0},{1}", state, (int)state);
            bleState = state;
            if (BleStateEvent != null) {
                BleStateEvent(state);
            }
        }

        /// デバイス状態変更通知を受け取る
        /// @param[in] state デバイスの状態
        /// @param[in] bleId デバイスのBLE-ID
        internal void OnNotifyDeviceState(HonokaDeviceState state, string bleId) {
            Debug.LogFormat("OnNotifyDeviceState/ state:{0},{1} id:{2}", state, (int)state, bleId);
            if (DeviceStateEvent != null) {
                DeviceStateEvent(state, bleId);
            } 
        }

        /// エラー通知を受け取る
        /// @param[in] errorCode エラーコード
        /// @param[in] bleId デバイスのBLE-ID
        internal void OnNotifyErrorCode(HonokaErrorCode errorCode, string bleId) {
            Debug.LogFormat("OnNotifyErrorCode/ errorCode:{0},{1} id:{2}", errorCode, (int)errorCode, bleId);
            if (ErrorEvent != null) {
                ErrorEvent(errorCode);
            }
        }

        /// デバイス検出通知を受け取る
        /// @param[in] bleId 検出したデバイスのBLE-ID
        /// @param[in] name 検出したデバイスの名前
        /// @param[in] rssi 検出時のRSSI
        internal void OnDiscoveredDevice(string bleId, string name, int rssi) {
            //Debug.LogFormat("OnDiscoveredDevice/ bleId:{0}, name:{1} rssi:{2}", bleId, name, rssi);
            DeviceInfo device = devices.Find(d => d.bleId.Equals(bleId)); 
            if (device == null) {
                device = new DeviceInfo(bleId, name, rssi);
                devices.Add(device);
            } else {
                device.name = name;
                device.rssi = rssi;
            }
            if (ScanEvent != null) {
                ScanEvent(bleId, name, rssi);
            }
        }

        //-------------------------------------------------------------------------

        /// Honokaデバイス BLE情報
        public class DeviceInfo {
            public string bleId; ///< BLE UUID
            public string name; ///< BLE Name
            public int rssi; ///< BLE受信信号強度 RSSI

            /// コンストラクタ
            public DeviceInfo(string bleId, string name, int rssi) {
                this.bleId = bleId;
                this.name = name;
                this.rssi = rssi;
            }
        }

        //-------------------------------------------------------------------------

        /// 状態変更・エラーなどの通知の受信処理 (HonokaLibに渡すコールバック関数)
        /// @param[in] type 通知タイプ
        /// @param[in] param1 通知データ１
        /// @param[in] param2 通知データ２
        [MonoPInvokeCallback(typeof(HonokaManagerNotifyCallback))]
        static void OnNotify(HonokaNotifyType type, int param1, string param2) {
            //Debug.LogFormat("OnNotify/ type:{0}, param:{1}", type, param);
            switch (type) {
                case HonokaNotifyType.BleState:
                    instance.OnNotifyBleState((HonokaBleState)Enum.ToObject(typeof(HonokaBleState), param1));
                    break;
                case HonokaNotifyType.DeviceState:
                    instance.OnNotifyDeviceState((HonokaDeviceState)Enum.ToObject(typeof(HonokaDeviceState), param1), param2);
                    break;
                case HonokaNotifyType.ErrorCode:
                    instance.OnNotifyErrorCode((HonokaErrorCode)Enum.ToObject(typeof(HonokaErrorCode), param1), param2);
                    break;
            }
        }

        /// デバイス検出の受信処理 (HonokaLibに渡すコールバック関数)
        /// @param[in] bleId 検出したデバイスの BLE-ID
        /// @param[in] name 検出したデバイスの名前
        /// @param[in] rssi 検出時のRSSI
        [MonoPInvokeCallback(typeof(HonokaManagerScanCallback))]
        static void OnScan(string bleId, string name, int rssi) {
            //Debug.LogFormat("OnScan/ bleId:{0}, name:{1} rssi:{2}", bleId, name, rssi);
            instance.OnDiscoveredDevice(bleId, name, rssi);
        }
    }
}