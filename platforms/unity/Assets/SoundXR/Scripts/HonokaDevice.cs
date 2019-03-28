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
/// @file HonokaDevice.cs
/// @brief HonokaDevice クラス 定義
///

using System;
using System.Threading;
using System.Runtime.InteropServices;
using UnityEngine;
using UnityEngine.Assertions;
using AOT;
using Soundxr.DeviceManager.Honoka.Plugin;


namespace Soundxr.DeviceManager.Honoka {

    /// Honokaデバイスクラス
    [AddComponentMenu("Sound xR/HonokaDevice")]
    public class HonokaDevice : MonoBehaviour {

        public Transform objectToRotate; ///< デバイスの姿勢を反映させるオブジェクト
        GameObject model; ///< objectToRotate の GameObject
        public bool attached { get; private set; } ///< アタッチ済み
        public bool connected { get; private set; } ///< 接続中

        String deviceId = null; ///< Attach したデバイスID
        IntPtr devicePtr = IntPtr.Zero; ///< HonokaLibから得るデバイスオブジェクトポインタ
        GCHandle handle; ///< thisをHonokaLibに渡すためのハンドル

        SynchronizationContext context = null; ///< 表示更新のために保持するコンテキスト
        Quaternion sensorQuaternion = Quaternion.identity; ///< デバイスから得た未補正のクォータニオン値
        Quaternion resetQuaternion = Quaternion.identity; ///< リセット用のクォータニオン値。補正値

        //-------------------------------------------------------------------------

        /// デバイス状態変更イベント
        public event Action<HonokaDeviceState> DeviceStateEvent;
        /// エラーイベント
        public event Action<HonokaErrorCode> ErrorEvent;
        /// 回転イベント（姿勢データ）
        public event Action<Quaternion> RotateEvent;

        //-------------------------------------------------------------------------

        /// 最初のフレームのアップデート前に呼び出される。初期化を行う
        void Start() {
            context = SynchronizationContext.Current;

            if (null != objectToRotate) {
                objectToRotate.rotation = Quaternion.identity;
                model = objectToRotate.gameObject;
                activeModel = false;
            }
        
            HonokaDeviceManager manager = HonokaDeviceManager.instance;
            Assert.IsNotNull(manager, "One HonokaDeviceManager is required.");

            attached = false;
            connected = false;
        }

        /// 破棄の前に呼び出される。終了処理を行う
        void OnDestroy(){
            Detach();
        }

        //-------------------------------------------------------------------------

        /// デバイスを割り当てる
        /// @param[in] deviceId デバイスID（BLE-ID）
        public void Attach(String deviceId) {
            if (attached) {
                return;
            }

            handle = GCHandle.Alloc(this);
            IntPtr ptr = GCHandle.ToIntPtr(handle);

            devicePtr = HonokaApi.GetDevice(deviceId, ptr);
            
            attached = (devicePtr != IntPtr.Zero); 
            if (attached) {
                this.deviceId = deviceId;
                connected = false;
            } else {
                handle.Free();
            }
        }

        /// デバイスの割り当てを解除する
        public void Detach() {
            if (!attached) {
                return;
            }
            HonokaApi.ReleaseDevice(devicePtr);
            devicePtr = IntPtr.Zero;
            deviceId = null;
            attached = false; 
            connected = false;
            handle.Free();
        }

        /// デバイスに接続する
        public void Connect() {
            if (!attached) {
                return;
            }
            HonokaApi.Connect(devicePtr, OnNotify, OnSensor);
        }

        /// デバイスとの接続を解除する
        public void Disconnect() {
            if (!attached) {
                return;
            }
            HonokaApi.Disconnect(devicePtr);
        }

        /// センシングを開始する
        /// @param[in] type センサタイプ
        public void StartSensing(HonokaSensorFlag type) {
            if (!attached && !connected) {
                return;
            }
            HonokaApi.StartSensing(devicePtr, type);
        }

        /// センシングを停止する
        /// @param[in] type センサタイプ
        public void StopSensing(HonokaSensorFlag type) {
            if (!attached && !connected) {
                return;
            }
            HonokaApi.StopSensing(devicePtr, type);
        }

        /// ノイズモードを設定する
        /// @param[in] mode ノイズモード（ANC・HT・Off）
        public void SetNoiseMode(HonokaNoiseMode mode) {
            if (!attached && !connected) {
                return;
            }
            HonokaApi.SetNoiseMode(devicePtr, mode);
        }

        /// 音量を上げる（＋１）
        public void TurnUpVolume() {
            if (!attached && !connected) {
                return;
            }
            HonokaApi.TurnUpVolume(devicePtr);
        }

        /// 音量を上げる（ー１）
        public void TurnDownVolume() {
            if (!attached && !connected) {
                return;
            }
            HonokaApi.TurnDownVolume(devicePtr);
        }

        /// 姿勢データをリセットする（現在の姿勢を単位回転にする）
        public void ResetAttitude() {
            if (!attached && !connected) {
                return;
            }
            resetQuaternion = Quaternion.Inverse(sensorQuaternion);

            OnChangedQuaternion();
        }

        /// 姿勢データを取得する
        /// @return 姿勢データ
        public Quaternion GetQuaternion() {
            return resetQuaternion * sensorQuaternion;
        }

        //-------------------------------------------------------------------------

        /// 姿勢データが変更された時の処理
        void OnChangedQuaternion() {
            Quaternion quaternion = resetQuaternion * sensorQuaternion;

            RotateModel(quaternion);
            if (RotateEvent != null) {
                RotateEvent(quaternion);
            }
        }

        /// モデルに姿勢データを設定する
        /// @param[in] quaternion モデルに設定する姿勢データ
        void RotateModel(Quaternion quaternion) {
            if (null == objectToRotate) {
                return;
            }
            if (null == SynchronizationContext.Current) {
                context.Post(__ => {
                    objectToRotate.rotation = quaternion;
                }, null);
            } else {
                objectToRotate.rotation = quaternion;
            }
        }

        /// objectToRotateオブジェクトのActive値（表示 or 非表示）
        bool activeModel {
            set {
                if (null == model) {
                    return;
                }
                if (null == SynchronizationContext.Current) {
                    context.Post(__ => { model.SetActive(value); }, null);
                } else {
                    model.SetActive(value);
                }
            }
            get {
                if (null == model) {
                    return false;
                }
                return model.activeSelf;
            }
        }

        /// 姿勢データを受信した時の処理。デバイス座標系をUnity座標系に変換する
        /// @param[in] quaternionRaw 姿勢データ(未補正)
        void OnRecievedQuaternion(Quaternion quaternionRaw) {
            //
            // The direction of coordinate systems for Honoka and Unity are different.
            // So, can have the following relation;
            //
            //    HONOKA           Unity            transfer result
            //     x(rear)          x(right)              z
            //     y(down)          y(up)                -y
            //     z(right)         z(front)             -x
            //     right-hand       left-hand            -θ
            //
            // notes: Q(a,b,c,w)|-θ = Q(-a,-b,-c,w)|θ
            //
            sensorQuaternion = new Quaternion(-quaternionRaw.z, quaternionRaw.y, quaternionRaw.x, quaternionRaw.w);

            OnChangedQuaternion();
        }

        //-------------------------------------------------------------------------

        /// デバイス状態変更通知の受信処理
        /// @param[in] state デバイスの状態
        internal void OnNotifyDeviceState(HonokaDeviceState state) {
            Debug.LogFormat("OnNotifyDeviceState/ state:{0},{1} id:{2}", state, (int)state, deviceId);
            switch (state) {
                case HonokaDeviceState.DidConnect:
                    connected = true;
                    break;
                case HonokaDeviceState.DidDisconnect:
                    connected = false;
                    break;
            }
            
            activeModel = connected;

            if (DeviceStateEvent != null) {
                DeviceStateEvent(state);
            } 
        }

        /// エラー通知の受信処理
        /// @param[in] errorCode エラーコード
        internal void OnNotifyErrorCode(HonokaErrorCode errorCode) {
            Debug.LogFormat("OnNotifyErrorCode/ errorCode:{0},{1} id:{2}", errorCode, (int)errorCode, deviceId);
            if (ErrorEvent != null) {
                ErrorEvent(errorCode);
            }
        }

        /// デバイスのセンサデータ受信処理
        internal void OnRecievedSensor(HonokaSensorState state, HonokaSensorFlag flag, Quaternion quaternion) {
            //Debug.LogFormat("OnBeansSensor/ bleId:{0}, state:{1} type:{2} quaternion:{3}", bleId, state, flag, quaternion);
            switch (state) {
                case HonokaSensorState.DidChange: 
                    if (flag == HonokaSensorFlag.Quaternion) {
                        OnRecievedQuaternion(quaternion);
                    }
                    break;
                case HonokaSensorState.DidStart:
                    break;
                case HonokaSensorState.DidStop:
                    break;
                case HonokaSensorState.FailedToStart:
                case HonokaSensorState.FailedToStop:
                case HonokaSensorState.FailedToRead:
                    break;
            }
        }

        //-------------------------------------------------------------------------

        /// 状態変更・エラーなどの通知の受信処理 (HonokaLibに渡すコールバック関数)
        /// @param[in] objectPtr HonokaDevice.Attach() でデバイスに結びつけた HonokaDevice のポインタ
        /// @param[in] type 通知タイプ ( DeviceState or ErrorCode )
        /// @param[in] param 通知データ
        ///  - type =　DeviceState の場合： HonokaDeviceState
        ///  - type =　ErrorCode の場合： HonokaErrorCode
        [MonoPInvokeCallback(typeof(HonokaDeviceNotifyCallback))]
        static void OnNotify(IntPtr objectPtr, HonokaNotifyType type, int param) {
            GCHandle handle = GCHandle.FromIntPtr(objectPtr);
            HonokaDevice device = handle.Target as HonokaDevice;
            if (device == null) {
                return;
            }
            //Debug.LogFormat("OnNotify/ type:{0}, param:{1}", type, param);
            switch (type) {
                case HonokaNotifyType.DeviceState:
                    device.OnNotifyDeviceState((HonokaDeviceState)Enum.ToObject(typeof(HonokaDeviceState), param));
                    break;
                case HonokaNotifyType.ErrorCode:
                    device.OnNotifyErrorCode((HonokaErrorCode)Enum.ToObject(typeof(HonokaErrorCode), param));
                    break;
            }
        }

        /// デバイスのセンサデータ受信処理 (HonokaLibに渡すコールバック関数)
        /// @param[in] objectPtr HonokaDevice.Attach() でデバイスに結びつけた HonokaDevice のポインタ
        /// @param[in] state センサの状態
        /// @param[in] flag センタタイプ
        /// @param[in] value0 センサデータ１
        /// @param[in] value1 センサデータ２ 
        /// @param[in] value2 センサデータ３
        /// @param[in] value3 センサデータ４
        /// @note 
        ///   value0 ~ value3 の値は、flag によって以下の意味となる
        ///   - flag = Quaternion の場合： Quaternion x, y, z, w
        [MonoPInvokeCallback(typeof(HonokaDeviceSensorCallback))]
        static void OnSensor(IntPtr objectPtr, HonokaSensorState state, HonokaSensorFlag flag, float value0, float value1, float value2, float value3) {
            //Debug.LogFormat("OnSensor/ state:{0} type:{1} value:{2}, {3}, {4}, {5}", state, flag, value0, value1, value2, value3);
            GCHandle handle = GCHandle.FromIntPtr(objectPtr);
            HonokaDevice device = handle.Target as HonokaDevice;
            if (device == null) {
                return;
            }
            device.OnRecievedSensor(state, flag, new Quaternion(value0, value1, value2, value3));
        }
    }
}
