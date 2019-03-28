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

/*****************************************************************************************//**
* @file             ViRealAudioSource.cs
* @brief            edit the parameters of the ViRealHeadphone spatializer
********************************************************************************************/

using UnityEngine;

namespace Soundxr.AudioEffectManager {

    /// ViReal 処理を編集するクラス。音源オブジェクトごとに add component する。
    [AddComponentMenu("Sound xR/ViRealAudioSource")]
    [RequireComponent(typeof(AudioSource))]
    public class ViRealAudioSource : MonoBehaviour {

        /// distance decay curve type
        public enum DecayCurve {
            slow = 0, ///< ゆるやかに減衰
            normal, ///< 標準の減衰
            fast ///< 急峻に減衰
        }

        // user parameters
        /// 出力音量 [dB] -96.0 ~ 20.0
        [Tooltip("set output volume [dB]")]
        [Range(-96.0f, 20.0f)]
        public float volume = 0.0f;

        /// 距離減衰On/Off
        [Tooltip("enable distance decay")]
        public bool distanceDecay = true;

        /// 距離減衰On時の距離減衰速度
        [Tooltip("set distance decay curve")]
        public DecayCurve decayCurve = DecayCurve.normal;

        //---

        // parameter index of the ViRealHeadphone Spatializer.
        private enum ParameterIndex {
            volume = 0,
            distanceDecay,
            decayCurve,
        }

        // default parameters
        private const bool distanceDecayDefault = true;
        private const DecayCurve decayCurveDefault = DecayCurve.normal;
        private const float volumeDefault = 0.0f;

        // previous values to detect the change of parameters
        private bool _distanceDecay = distanceDecayDefault;
        private DecayCurve _decayCurve = decayCurveDefault;
        private float _volume = volumeDefault;
        private bool _spatialize = false;

        // Use this for initialization
        private void Start() {
            // Unity's default sample rate for iOS/Android is 24kHz.
            // we strongly recommend setting to 48kHz.
            AudioConfiguration config = AudioSettings.GetConfiguration();
            if (config.sampleRate != 48000) {
                config.sampleRate = 48000;
                AudioSettings.Reset(config);
                GetComponent<AudioSource>().Play();
            }
        }

        // Update is called once per frame
        void Update() {
            // check the existence of the audio source.
            AudioSource audioSource = gameObject.GetComponent<AudioSource>();
            if (!audioSource) {
                Debug.Log("ViRealAudioSource.cs needs an AudioSource, but not find.");
                return;
            }

            // do nothing if the spatialize property is off.
            if (!audioSource.spatialize) {
                _spatialize = false;
                return;
            }

            // update only if the value is changed.
            if (_volume != volume || _spatialize != audioSource.spatialize ) {
                if (audioSource.SetSpatializerFloat((int)ParameterIndex.volume, volume))
                    _volume = volume;
            }
            if (_distanceDecay != distanceDecay || _spatialize != audioSource.spatialize) {
                if (audioSource.SetSpatializerFloat((int)ParameterIndex.distanceDecay, distanceDecay ? 1.0f : 0.0f))
                    _distanceDecay = distanceDecay;
            }
            if (_decayCurve != decayCurve || _spatialize != audioSource.spatialize) {
                if (audioSource.SetSpatializerFloat((int)ParameterIndex.decayCurve, (float)decayCurve))
                    _decayCurve = decayCurve;
            }
            _spatialize = audioSource.spatialize;
        }
    }
}
/*********************************************************************************************
* EOF
********************************************************************************************/
