xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 32;
 -11.58056;-23.13131;-52.23116;,
 -11.58056;-0.12698;-14.55889;,
 11.55599;-0.12698;-14.55889;,
 11.55599;-23.13131;-52.23116;,
 -11.58056;-0.12698;-14.55889;,
 -11.58056;-0.12698;16.46832;,
 11.55599;-0.12698;16.46832;,
 11.55599;-0.12698;-14.55889;,
 -11.58056;-0.12698;16.46832;,
 -11.58056;-21.50101;16.46832;,
 11.55599;-21.50101;16.46832;,
 11.55599;-0.12698;16.46832;,
 -11.58056;-32.66721;16.46832;,
 -11.58056;-32.66721;-58.21724;,
 11.55599;-32.66721;-58.21724;,
 11.55599;-32.66721;16.46832;,
 11.55599;-23.13131;-52.23116;,
 11.55599;-0.12698;-14.55889;,
 11.55599;-0.12698;16.46832;,
 11.55599;-21.50101;16.46832;,
 -11.58056;-21.50101;16.46832;,
 -11.58056;-0.12698;16.46832;,
 -11.58056;-0.12698;-14.55889;,
 -11.58056;-23.13131;-52.23116;,
 -11.58056;-32.66721;-58.21724;,
 11.55599;-32.66721;-58.21724;,
 11.55599;-32.66721;-58.21724;,
 11.55599;-32.66721;16.46832;,
 -11.58056;-32.66721;16.46832;,
 11.55599;-32.66721;16.46832;,
 -11.58056;-32.66721;16.46832;,
 -11.58056;-32.66721;-58.21724;;
 
 10;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;16,17,18,19;,
 4;20,21,22,23;,
 4;24,0,3,25;,
 4;26,16,19,27;,
 4;9,28,29,10;,
 4;30,20,23,31;;
 
 MeshMaterialList {
  1;
  10;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\MATERIAL\\arms_00.png";
   }
  }
 }
 MeshNormals {
  8;
  0.000000;-1.000000;-0.000000;,
  0.000000;0.962668;-0.270685;,
  0.000000;1.000000;0.000000;,
  0.000000;0.711463;-0.702724;,
  0.000000;-0.000000;1.000000;,
  1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;0.531668;-0.846953;;
  10;
  4;3,1,1,3;,
  4;1,2,2,1;,
  4;4,4,4,4;,
  4;0,0,0,0;,
  4;5,5,5,5;,
  4;6,6,6,6;,
  4;7,3,3,7;,
  4;5,5,5,5;,
  4;4,4,4,4;,
  4;6,6,6,6;;
 }
 MeshTextureCoords {
  32;
  0.495156;0.812189;,
  0.535079;0.863924;,
  0.503306;0.888443;,
  0.463382;0.836708;,
  0.554232;0.957248;,
  0.554232;0.877387;,
  0.613783;0.877387;,
  0.613783;0.957248;,
  0.496874;0.965417;,
  0.496874;0.928341;,
  0.537008;0.928341;,
  0.537008;0.965417;,
  0.398036;0.964155;,
  0.398036;0.834602;,
  0.438170;0.834602;,
  0.438170;0.964155;,
  0.456169;0.844986;,
  0.496074;0.910334;,
  0.496074;0.964155;,
  0.458997;0.964155;,
  0.457779;0.964155;,
  0.494855;0.964155;,
  0.494855;0.910334;,
  0.454951;0.844986;,
  0.488812;0.803968;,
  0.457039;0.828487;,
  0.439628;0.834602;,
  0.439628;0.964155;,
  0.496874;0.908972;,
  0.537008;0.908972;,
  0.438409;0.964155;,
  0.438409;0.834602;;
 }
}
