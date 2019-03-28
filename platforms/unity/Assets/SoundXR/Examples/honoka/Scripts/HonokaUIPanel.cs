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
/// @file HonokaUIPanel.cs
/// @brief HonokaUIPanel クラス 定義
///

using UnityEngine;
using System;
using Soundxr.DeviceManager.Honoka;
using Soundxr.DeviceManager.Honoka.Plugin;


/// Honokaデバイスのデモ画面
[AddComponentMenu("Sound xR/Samples/HonokaUIPanel")]
[RequireComponent(typeof(HonokaDeviceManager))]
[RequireComponent(typeof(HonokaDevice))]
public class HonokaUIPanel : MonoBehaviour
{
    HonokaDeviceManager honokaManager; ///< Honoka管理オブジェクト
    HonokaDevice honokaDevice; ///< Honokaデバイスオブジェクト
    HonokaBleState bleState = HonokaBleState.Unknown; ///< BLEの状態 (HonokaDeviceManagerから通知された最終値)
    HonokaDeviceState deviceState = HonokaDeviceState.DidDisconnect; ///< デバイスの状態
    string targetBleId = null; ///< 選択中デバイスのBLE-ID (切断または接続失敗したらnull)
    HonokaNoiseMode noiseMode = HonokaNoiseMode.Off; ///< ノイズモード
    Vector2 scrollPositionOfList = new Vector2(0, 0); ///< 検出デバイスのボタンのスクロール位置
    Vector2 scrollPositionOfLog = new Vector2(0, 0); ///< ログエリアのスクロール位置
    string logText = ""; ///< 画面表示するログテキスト
    string quaternionText = ""; ///< 画面表示する姿勢情報テキスト

    /// 最初のフレームのアップデート前に呼び出される。初期化を行う
    void Start() {
        honokaManager = HonokaDeviceManager.instance;
        if (honokaManager) {
            honokaManager.BleStateEvent += OnBleStateEvent;
            honokaManager.DeviceStateEvent += OnDeviceStateEvent;
            honokaManager.ErrorEvent += OnErrorEvent;
        }
    
        honokaDevice = gameObject.GetComponent<HonokaDevice>();
    }

    /// ラベルとボタンの表示とハンドリングを行う
    void OnGUI() {
        GUI.enabled = true;

        if (honokaManager && honokaDevice) { // 上のエリア
            GUILayout.BeginArea(new Rect(20, 0, Screen.width - 40, Screen.height / 2));
            GUILayout.Space(35);
            if (deviceState == HonokaDeviceState.DidConnect) {
                {
                    GUILayout.BeginHorizontal();
                    GUILayout.FlexibleSpace();
                    if (GUILayout.Button("Disconnect")) {
                        honokaDevice.Disconnect();
                        honokaDevice.Detach();
                    }
                    GUILayout.EndHorizontal();
                }
                GUILayout.FlexibleSpace();
                {
                    GUILayout.BeginHorizontal();
                    GUILayout.FlexibleSpace();
                    GUILayout.Label(quaternionText);
                    GUILayout.EndHorizontal();
                }
                {
                    GUILayout.BeginHorizontal();
                    GUILayout.FlexibleSpace();
                    GUILayout.Label("Sensing");
                    if (GUILayout.Button("On", GUILayout.MinWidth(50))) {
                        honokaDevice.StartSensing(HonokaSensorFlag.Quaternion);
                    }
                    if (GUILayout.Button("Off", GUILayout.MinWidth(50))) {
                        honokaDevice.StopSensing(HonokaSensorFlag.Quaternion);
                    }
                    GUILayout.Space(10);
                    if (GUILayout.Button("Reset", GUILayout.MinWidth(70))) {
                        honokaDevice.ResetAttitude();
                    }
                    GUILayout.EndHorizontal();
                }
                {
                    GUILayout.BeginHorizontal();
                    GUILayout.FlexibleSpace();
                    GUILayout.Label("DynamicHT");
                    GUI.enabled = noiseMode != HonokaNoiseMode.ANC;
                    if (GUILayout.Button("ANC", GUILayout.MinWidth(50))) {
                        noiseMode = HonokaNoiseMode.ANC;
                        honokaDevice.SetNoiseMode(noiseMode);
                    }
                    GUI.enabled = noiseMode != HonokaNoiseMode.HT;
                    if (GUILayout.Button("HT", GUILayout.MinWidth(50))) {
                        noiseMode = HonokaNoiseMode.HT;
                        honokaDevice.SetNoiseMode(noiseMode);
                    }
                    GUI.enabled = noiseMode != HonokaNoiseMode.Off;
                    if (GUILayout.Button("Off", GUILayout.MinWidth(50))) {
                        noiseMode = HonokaNoiseMode.Off;
                        honokaDevice.SetNoiseMode(noiseMode);
                    }
                    GUILayout.EndHorizontal();
                }
                {
                    GUI.enabled = true;
                    GUILayout.BeginHorizontal();
                    GUILayout.FlexibleSpace();
                    GUILayout.Label("Volume");
                    if (GUILayout.Button("Up", GUILayout.MinWidth(50))) {
                        honokaDevice.TurnUpVolume();
                    }
                    if (GUILayout.Button("Down", GUILayout.MinWidth(50))) {
                        honokaDevice.TurnDownVolume();
                    }
                    GUILayout.EndHorizontal();
                }
            } else {
                {
                    GUILayout.BeginHorizontal();
                    GUI.enabled = (bleState == HonokaBleState.PoweredOn) && (targetBleId == null);
                    if (!honokaManager.scanning && GUILayout.Button("Scan", GUILayout.MinWidth(70))) {
                        honokaManager.StartScan();
                    }
                    if (honokaManager.scanning && GUILayout.Button("Stop", GUILayout.MinWidth(70))) {
                        honokaManager.StopScan();
                    }
                    string label = honokaManager.scanning ? "Scanning..." : "";
                    GUILayout.Label(label);

                    GUILayout.FlexibleSpace();
                    GUI.enabled = (bleState == HonokaBleState.PoweredOn);
                    if (targetBleId != null && GUILayout.Button("Cancel", GUILayout.MinWidth(70))) {
                        honokaDevice.Disconnect();
                    }
                    GUILayout.EndHorizontal();
                }
                GUILayout.Space(10);
                {
                    GUI.enabled = (bleState == HonokaBleState.PoweredOn) && (targetBleId == null);
                    scrollPositionOfList = GUILayout.BeginScrollView(scrollPositionOfList);
                    for (int i = 0; i < honokaManager.devices.Count; i++) {
                        var rssi = honokaManager.devices[i].rssi;
                        var text = honokaManager.devices[i].name + " ( " + ((rssi == 127) ? " - " : rssi.ToString()) + " )";
                        if (Application.platform == RuntimePlatform.Android) {
                            text = "[" + honokaManager.devices[i].bleId + "] " + text; 
                        }
                        if (GUILayout.Button(text)) {
                            targetBleId = honokaManager.devices[i].bleId;
                            honokaManager.StopScan();
                            honokaDevice.ErrorEvent += OnDeviceErrorEvent;
                            honokaDevice.RotateEvent += OnDeviceRotateEvent;
                            honokaDevice.Attach(targetBleId);
                            honokaDevice.Connect();
                            quaternionText = "";
                        }
                    }
                    GUILayout.EndScrollView();
                }
            }
            GUILayout.Space(10);
            GUILayout.EndArea();
        }

        GUI.enabled = true;
        { // 下エリアに、ログを表示
            GUILayout.BeginArea(new Rect(20, Screen.height / 2, Screen.width -40, Screen.height / 2));
            GUILayout.Space(10);
            {
                GUILayout.BeginVertical("box");
                {
                    scrollPositionOfLog = GUILayout.BeginScrollView(scrollPositionOfLog);
                    GUILayout.Label(logText);
                    GUILayout.EndScrollView();
                }
                GUILayout.FlexibleSpace();
                GUILayout.EndVertical();
            }
            if (GUILayout.Button("Clear log", GUILayout.ExpandWidth(false))) {
                logText = "";
            }
            GUILayout.Space(20);
            GUILayout.EndArea();
        }
    }

    /// HonokaManager の BleStateEvent 処理
    void OnBleStateEvent(HonokaBleState state) {
        Print("BleStateEvent: " + state);
        bleState = state;
        if (state == HonokaBleState.Resetting) {
            honokaManager.StopScan();
        }
    }

    /// HonokaManager の DeviceStateEvent 処理
    void OnDeviceStateEvent(HonokaDeviceState state, string bleId) {
        Print("DeviceStateEvent: " + state + "(" + (int)state + ") " + bleId);
        deviceState = state;
        if (state == HonokaDeviceState.DidDisconnect) {
            targetBleId = null;
        }
    }

    /// HonokaManager の ErrorEvent 処理
    void OnErrorEvent(HonokaErrorCode errorCode) {
        Print("ErrorEvent: " + errorCode + " " + (int)errorCode);
        targetBleId = null;
    }

    /// HonokaDevice の RotateEvent 処理
    void OnDeviceRotateEvent(Quaternion q) {
        quaternionText = "x:" + q.x.ToString("F6") + " y:" + q.y.ToString("F6") + " z:" + q.z.ToString("F6") + " w:" + q.w.ToString("F6");
    }

    /// HonokaDevice の ErrorEvent 処理
    void OnDeviceErrorEvent(HonokaErrorCode errorCode) {
        Print("DeviceErrorEvent: " + errorCode + " " + (int)errorCode);
    }

    /// ログを出力する
    void Print(String log) {
        logText += log + "\n";
    }
}
