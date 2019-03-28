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
/// @file MeshPyramid.cs
/// @brief MeshPyramid クラス 定義
///

using UnityEngine;
using System.Collections.Generic; // List
using System.Linq; // Enumerable

/// 多角錐 3D Object
[AddComponentMenu("Sound xR/Samples/MeshPyramid")]
[RequireComponent(typeof(MeshRenderer))]
[RequireComponent(typeof(MeshFilter))]
public class MeshPyramid : MonoBehaviour {

    /// 多角錐の高さ
    public float height = 0.8f;

    /// 多角錐の半径
    public float radius = 0.4f;

    /// 多角錐の分割数
    public int fractionSize = 8;

    /// 多角錐メッシュを生成
    private Mesh Pyramid() {

        Vector3[] bottom = new Vector3[fractionSize];
        List<Vector3> vtx = new List<Vector3>();

        // 頂点生ベクトルを成
        for (int i = 0; i < fractionSize; i++) {
            float rad = i * 2 * Mathf.PI / fractionSize;
            Vector3 v = new Vector3(Mathf.Cos(rad), Mathf.Sin(rad), 0);
            bottom[i] = v * radius;
        }

        // ベクトル形式でモデル化
        for (int i = 0; i < fractionSize; i++) {
            int ii = i + 1;
            if (ii >= fractionSize)
                ii = 0;
            Vector3 v_bot = bottom[ii];
            vtx.AddRange(new Vector3[] { Vector3.zero, v_bot, bottom[i] }); // 底面
            vtx.AddRange(new Vector3[] { bottom[i], v_bot, Vector3.forward * height }); // 側面
        }
        // メッシュ化
        Mesh mesh = new Mesh {
            vertices = vtx.ToArray(),
            triangles = Enumerable.Range(0, vtx.Count).ToArray()
        };
        mesh.RecalculateNormals();
        mesh.RecalculateBounds();

        return mesh;
    }

    private void Start() {
        GetComponent<MeshFilter>().sharedMesh = Pyramid();
        GetComponent<MeshRenderer>().material = new Material(Shader.Find("Diffuse"));
    }
}
