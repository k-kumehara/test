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
* @file             ViRealAudioSourceEditor.cs
* @brief            Custom Inspector Editor for ViRealAudioSource.cs
* @attention        
********************************************************************************************/

using UnityEngine;
using UnityEditor;

namespace Soundxr.AudioEffectManager {

// Custom Editor using SerializedProperties
// automatic handling of multi-object editing, undo, and Prefab overrides
[CustomEditor(typeof(ViRealAudioSource))]
[CanEditMultipleObjects]
public class ViRealAudioSourceEditor : Editor {

    // properties and labels
    private SerializedProperty m_propVolume = null;
    private SerializedProperty m_propDistanceDecay = null;
    private SerializedProperty m_propDecayCurve = null;
    private GUIContent m_labelVolume = new GUIContent("Volume [dB]");
    private GUIContent m_labelDistanceDecay = new GUIContent("Distance Decay");
    private GUIContent m_labelDecayCurve = new GUIContent("Decay Curve");

    private void OnEnable() {
        m_propVolume = serializedObject.FindProperty("volume");
        m_propDistanceDecay = serializedObject.FindProperty("distanceDecay");
        m_propDecayCurve = serializedObject.FindProperty("decayCurve");
    }

    private void OnDisable() {
        m_propDistanceDecay = null;
        m_propDecayCurve = null;
        m_propVolume = null;
    }

    public override void OnInspectorGUI() {
        // update the serializedProperty
        serializedObject.Update();

        // show the custom GUI controls
        EditorGUILayout.Separator();
        EditorGUILayout.PropertyField(m_propVolume, m_labelVolume);
        EditorGUILayout.Separator();
        EditorGUILayout.PropertyField(m_propDistanceDecay, m_labelDistanceDecay);
        if (m_propDistanceDecay.boolValue) {
            EditorGUI.indentLevel++;
            EditorGUILayout.PropertyField(m_propDecayCurve, m_labelDecayCurve);
            EditorGUI.indentLevel--;
        }

        // apply changes to the serializedProperty
        serializedObject.ApplyModifiedProperties();
    }
}

}

/*********************************************************************************************
* EOF
********************************************************************************************/