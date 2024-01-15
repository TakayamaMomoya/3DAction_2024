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
 324;
 34.69641;-13.55680;9.50821;,
 7.77240;-13.48927;9.50821;,
 7.94303;3.76541;9.50821;,
 39.86292;3.29962;9.50821;,
 8.41756;-15.78695;18.57818;,
 7.71501;-18.98714;13.76807;,
 32.99313;-19.11398;13.76807;,
 29.97481;-15.73257;18.57818;,
 1.05145;-11.85987;13.76807;,
 4.52838;-11.28611;18.57818;,
 4.49386;2.39597;18.57818;,
 0.72207;3.84546;13.76807;,
 30.59956;-9.97053;17.26918;,
 33.26605;-11.97309;13.26585;,
 37.56597;2.05587;13.26585;,
 34.43100;1.59658;17.26918;,
 7.96206;13.03600;13.76807;,
 8.35338;9.64806;18.57818;,
 29.91064;9.70247;18.57818;,
 32.91185;13.09900;13.76807;,
 8.49031;2.44753;18.57818;,
 8.40672;-11.26602;18.57818;,
 31.49252;-11.15065;18.57818;,
 36.09617;2.74771;18.57818;,
 7.88487;13.03583;9.50821;,
 7.96206;13.03600;13.76807;,
 32.91185;13.09900;13.76807;,
 32.91185;13.09900;9.50821;,
 7.71501;-18.98714;13.76807;,
 7.71501;-18.98714;9.50821;,
 32.99313;-19.11398;9.50821;,
 32.99313;-19.11398;13.76807;,
 34.69641;-13.55680;13.76807;,
 34.69641;-13.55680;9.50821;,
 39.86292;3.29962;9.50821;,
 39.86292;3.29962;13.76807;,
 1.05145;-11.85987;9.50821;,
 1.05145;-11.85987;13.76807;,
 0.72207;3.84546;13.76807;,
 0.72207;3.84546;9.50821;,
 7.93260;10.28013;9.50821;,
 34.97815;10.18599;9.50821;,
 34.97815;10.18599;9.50821;,
 34.97815;10.18599;13.76807;,
 39.86292;3.29962;13.76807;,
 34.97815;10.18599;13.76807;,
 31.74938;7.63504;18.57818;,
 8.40947;7.50759;18.57818;,
 4.48101;7.48537;18.57818;,
 0.70580;10.29108;13.76807;,
 0.70580;10.29108;13.76807;,
 0.70580;10.29108;9.50821;,
 29.97481;-15.73257;18.57818;,
 32.99313;-19.11398;13.76807;,
 31.84846;-16.59811;13.26585;,
 29.33641;-13.78393;17.26918;,
 34.69641;-13.55680;13.76807;,
 39.86292;3.29962;13.76807;,
 36.09617;2.74771;18.57818;,
 31.49252;-11.15065;18.57818;,
 7.93260;10.28013;-9.41253;,
 7.88487;13.03583;-9.41253;,
 32.91185;13.09900;-9.41253;,
 34.97815;10.18599;-9.41253;,
 32.91185;13.09900;9.50821;,
 32.91185;13.09900;13.76807;,
 4.47558;9.63825;18.57818;,
 0.69893;13.01769;13.76807;,
 0.69893;13.01769;13.76807;,
 0.69893;13.01769;9.50821;,
 0.69893;13.01769;13.76807;,
 0.69893;13.01769;9.50821;,
 6.48034;-13.59859;18.57818;,
 4.48163;-15.64041;13.76807;,
 4.48163;-15.64041;9.50821;,
 7.71501;-18.98714;9.50821;,
 7.71501;-18.98714;13.76807;,
 4.48163;-15.64041;13.76807;,
 0.72207;3.84546;-9.41253;,
 7.94303;3.76541;-9.31685;,
 7.77236;-13.48928;-9.31685;,
 1.05143;-11.85987;-9.41253;,
 0.70580;10.29108;-9.41253;,
 0.69895;13.01769;-9.41253;,
 32.99313;-19.11398;13.76807;,
 32.99313;-19.11398;9.50821;,
 7.71501;-18.98714;9.50821;,
 32.99313;-19.11398;9.50821;,
 7.71497;-18.98714;-9.41253;,
 4.48160;-15.64043;-9.41253;,
 34.69641;-13.55680;-9.50823;,
 39.86292;3.29962;-9.50823;,
 7.94303;3.76541;-9.50823;,
 7.77240;-13.48927;-9.50821;,
 8.41756;-15.78695;-18.57819;,
 29.97481;-15.73257;-18.57819;,
 32.99313;-19.11398;-13.76808;,
 7.71501;-18.98714;-13.76808;,
 1.05145;-11.85987;-13.76808;,
 0.72207;3.84546;-13.76808;,
 4.49386;2.39597;-18.57819;,
 4.52838;-11.28611;-18.57818;,
 30.59956;-9.97053;-17.26917;,
 34.43100;1.59658;-17.26917;,
 37.56597;2.05587;-13.26588;,
 33.26605;-11.97309;-13.26588;,
 7.96206;13.03600;-13.76808;,
 32.91185;13.09900;-13.76808;,
 29.91064;9.70247;-18.57819;,
 8.35338;9.64806;-18.57819;,
 8.49031;2.44753;-18.57819;,
 36.09617;2.74771;-18.57819;,
 31.49252;-11.15065;-18.57819;,
 8.40672;-11.26602;-18.57819;,
 7.88487;13.03583;-9.41253;,
 32.91185;13.09900;-9.41253;,
 32.91185;13.09900;-13.76808;,
 7.96206;13.03600;-13.76808;,
 7.71501;-18.98714;-13.76808;,
 32.99313;-19.11398;-13.76808;,
 32.99313;-19.11398;-9.50823;,
 7.71497;-18.98714;-9.41253;,
 34.69641;-13.55680;-13.76808;,
 39.86292;3.29962;-13.76808;,
 39.86292;3.29962;-9.50823;,
 34.69641;-13.55680;-9.50823;,
 1.05143;-11.85987;-9.41253;,
 0.72207;3.84546;-9.41253;,
 0.72207;3.84546;-13.76808;,
 1.05145;-11.85987;-13.76808;,
 34.97815;10.18599;-9.41253;,
 7.93260;10.28013;-9.41253;,
 34.97815;10.18599;-13.76808;,
 34.97815;10.18599;-9.41253;,
 31.74938;7.63504;-18.57819;,
 34.97815;10.18599;-13.76808;,
 39.86292;3.29962;-13.76808;,
 8.40947;7.50759;-18.57819;,
 0.70580;10.29108;-13.76808;,
 4.48101;7.48537;-18.57819;,
 0.70580;10.29108;-9.41253;,
 0.70580;10.29108;-13.76808;,
 29.97481;-15.73257;-18.57819;,
 29.33641;-13.78393;-17.26917;,
 31.84846;-16.59811;-13.26588;,
 32.99313;-19.11398;-13.76808;,
 39.86292;3.29962;-13.76808;,
 34.69641;-13.55680;-13.76808;,
 36.09617;2.74771;-18.57819;,
 31.49252;-11.15065;-18.57819;,
 32.91185;13.09900;-13.76808;,
 32.91185;13.09900;-9.41253;,
 0.69893;13.01769;-13.76808;,
 4.47558;9.63825;-18.57819;,
 0.69895;13.01769;-9.41253;,
 0.69893;13.01769;-13.76808;,
 0.69893;13.01769;-13.76808;,
 0.69895;13.01769;-9.41253;,
 6.48034;-13.59859;-18.57819;,
 4.48163;-15.64041;-13.76808;,
 4.48160;-15.64043;-9.41253;,
 4.48163;-15.64041;-13.76808;,
 7.71501;-18.98714;-13.76808;,
 7.71497;-18.98714;-9.41253;,
 7.77240;-13.48927;-9.50821;,
 7.94303;3.76541;-9.50823;,
 32.99313;-19.11398;-13.76808;,
 32.99313;-19.11398;-9.50823;,
 7.71497;-18.98714;-9.41253;,
 32.99313;-19.11398;-9.50823;,
 7.93260;10.28013;9.50821;,
 7.93260;10.28013;-9.41253;,
 34.97815;10.18599;-9.41253;,
 34.97815;10.18599;9.50821;,
 7.77240;-13.48927;9.50821;,
 7.94303;3.76541;9.50821;,
 7.93260;10.28013;9.50821;,
 7.71501;-18.98714;9.50821;,
 17.92515;16.57753;-8.41757;,
 33.60351;16.71120;-8.41757;,
 33.18558;14.80001;-8.41757;,
 17.70350;14.99954;-8.41757;,
 17.92515;16.57753;8.08360;,
 33.60377;16.59847;8.08360;,
 33.60351;16.71120;-8.41757;,
 17.92515;16.57753;-8.41757;,
 33.18558;14.80001;8.08360;,
 33.60377;16.59847;8.08360;,
 17.92515;16.57753;8.08360;,
 17.70350;14.99954;8.08360;,
 16.20545;-9.07742;-8.41757;,
 29.34963;-9.05334;-8.41757;,
 29.34963;-9.05334;8.08360;,
 16.20545;-9.07742;8.08360;,
 41.42898;11.73876;-6.32432;,
 41.80233;14.12868;-6.32432;,
 41.80233;14.12868;5.99033;,
 41.42898;11.73876;5.99033;,
 0.69897;13.17059;8.08360;,
 3.32633;16.50534;8.08360;,
 3.32633;16.50534;-8.41757;,
 0.69897;13.17059;-8.41757;,
 38.61103;-9.03635;-8.41757;,
 40.34142;-7.57652;-7.07647;,
 40.34142;-7.57652;6.74247;,
 38.61103;-9.03635;8.08360;,
 45.26600;11.26890;6.74247;,
 45.97884;13.99677;6.74247;,
 44.89155;16.65363;8.08360;,
 44.09739;13.40525;8.08360;,
 45.97884;13.99677;-7.07647;,
 44.89155;16.65363;-8.41757;,
 45.26600;11.26890;-7.07647;,
 44.09739;13.40525;-8.41757;,
 40.58377;-6.31495;-6.32432;,
 40.58377;-6.31495;5.99033;,
 45.19331;10.51361;5.99033;,
 45.86052;12.94950;5.99033;,
 45.86052;12.94950;-6.32432;,
 45.19331;10.51361;-6.32432;,
 40.58377;-6.31495;-6.32432;,
 38.84962;-4.77207;-6.32432;,
 38.84962;-4.77207;5.99033;,
 40.58377;-6.31495;5.99033;,
 41.42898;11.73876;5.99033;,
 41.80233;14.12868;5.99033;,
 45.86052;12.94950;5.99033;,
 45.19331;10.51361;5.99033;,
 41.80233;14.12868;-6.32432;,
 45.86052;12.94950;-6.32432;,
 41.42898;11.73876;-6.32432;,
 45.19331;10.51361;-6.32432;,
 7.43956;-9.09346;8.08360;,
 7.43956;-9.09346;-8.41757;,
 3.32633;16.50534;8.08360;,
 0.69897;13.17059;8.08360;,
 0.69897;13.17059;-8.41757;,
 3.32633;16.50534;-8.41757;,
 44.89155;16.65363;-8.41757;,
 44.09739;13.40525;-8.41757;,
 44.89155;16.65363;8.08360;,
 44.89155;16.65363;-8.41757;,
 44.09739;13.40525;8.08360;,
 44.89155;16.65363;8.08360;,
 38.61103;-9.03635;-8.41757;,
 38.61103;-9.03635;8.08360;,
 7.43956;-9.09346;-8.41757;,
 16.20545;-9.07742;-8.41757;,
 29.34963;-9.05334;-8.41757;,
 38.61103;-9.03635;-8.41757;,
 38.84962;-4.77207;-6.32432;,
 38.84962;-4.77207;5.99033;,
 29.34963;-9.05334;8.08360;,
 38.61103;-9.03635;8.08360;,
 16.20545;-9.07742;8.08360;,
 7.43956;-9.09346;8.08360;,
 7.43956;-9.09346;8.08360;,
 0.69897;13.17059;8.08360;,
 0.69897;13.17059;-8.41757;,
 7.43956;-9.09346;-8.41757;,
 29.68987;-13.71164;-17.26532;,
 35.05969;1.87408;-17.44657;,
 36.19551;1.94078;-18.37274;,
 30.82702;-14.16121;-18.19148;,
 38.69373;2.29578;-13.75426;,
 39.82930;2.47917;-14.68041;,
 31.92833;-16.12789;-13.24861;,
 33.06547;-16.57749;-14.17477;,
 30.83927;-12.85095;-17.18112;,
 35.53010;1.25982;-17.28998;,
 38.44503;1.88028;-14.52324;,
 32.59360;-14.78352;-13.87109;,
 31.92833;-16.12789;-13.24861;,
 38.69373;2.29578;-13.75426;,
 35.05969;1.87408;-17.44657;,
 29.68987;-13.71164;-17.26532;,
 30.82702;-14.16121;-18.19148;,
 31.46000;-13.07659;-17.70681;,
 33.21435;-15.00916;-14.39679;,
 33.06547;-16.57749;-14.17477;,
 39.06575;1.65466;-15.04896;,
 39.82930;2.47917;-14.68041;,
 36.15085;1.03420;-17.81568;,
 36.19551;1.94078;-18.37274;,
 31.46000;-13.07659;-17.70681;,
 30.83927;-12.85095;-17.18112;,
 32.59360;-14.78352;-13.87109;,
 33.21435;-15.00916;-14.39679;,
 38.44503;1.88028;-14.52324;,
 39.06575;1.65466;-15.04896;,
 35.53010;1.25982;-17.28998;,
 36.15085;1.03420;-17.81568;,
 29.68987;-13.71164;17.26530;,
 30.82702;-14.16121;18.19148;,
 36.19551;1.94078;18.37272;,
 35.05969;1.87408;17.44656;,
 39.82930;2.47917;14.68040;,
 38.69373;2.29578;13.75423;,
 33.06547;-16.57749;14.17476;,
 31.92833;-16.12789;13.24860;,
 30.83927;-12.85095;17.18113;,
 32.59360;-14.78352;13.87107;,
 38.44503;1.88028;14.52322;,
 35.53010;1.25982;17.28997;,
 31.92833;-16.12789;13.24860;,
 29.68987;-13.71164;17.26530;,
 35.05969;1.87408;17.44656;,
 38.69373;2.29578;13.75423;,
 30.82702;-14.16121;18.19148;,
 33.06547;-16.57749;14.17476;,
 33.21435;-15.00916;14.39676;,
 31.46000;-13.07659;17.70680;,
 39.82930;2.47917;14.68040;,
 39.06575;1.65466;15.04895;,
 36.19551;1.94078;18.37272;,
 36.15085;1.03420;17.81568;,
 31.46000;-13.07659;17.70680;,
 33.21435;-15.00916;14.39676;,
 32.59360;-14.78352;13.87107;,
 30.83927;-12.85095;17.18113;,
 39.06575;1.65466;15.04895;,
 38.44503;1.88028;14.52322;,
 36.15085;1.03420;17.81568;,
 35.53010;1.25982;17.28997;;
 
 172;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;16,17,18,19;,
 4;20,21,22,23;,
 4;24,25,26,27;,
 4;28,29,30,31;,
 4;32,33,34,35;,
 4;36,37,38,39;,
 4;2,40,41,3;,
 4;35,34,42,43;,
 4;23,44,45,46;,
 4;47,20,23,46;,
 4;11,10,48,49;,
 4;39,38,50,51;,
 4;52,53,54,55;,
 4;13,56,57,14;,
 4;57,58,15,14;,
 4;58,59,12,15;,
 4;60,61,62,63;,
 4;43,42,64,65;,
 4;46,45,19,18;,
 4;17,47,46,18;,
 4;49,48,66,67;,
 4;51,50,68,69;,
 4;70,25,24,71;,
 4;67,66,17,16;,
 4;48,47,17,66;,
 4;10,20,47,48;,
 4;9,21,20,10;,
 4;72,73,5,4;,
 4;74,75,76,77;,
 4;78,79,80,81;,
 4;82,60,79,78;,
 4;83,61,60,82;,
 4;72,4,21,9;,
 4;22,21,4,7;,
 4;12,59,52,55;,
 4;55,54,13,12;,
 4;53,56,13,54;,
 4;84,85,33,32;,
 4;86,1,0,87;,
 4;81,80,88,89;,
 4;74,77,37,36;,
 4;73,72,9,8;,
 4;90,91,92,93;,
 4;94,95,96,97;,
 4;98,99,100,101;,
 4;102,103,104,105;,
 4;106,107,108,109;,
 4;110,111,112,113;,
 4;114,115,116,117;,
 4;118,119,120,121;,
 4;122,123,124,125;,
 4;126,127,128,129;,
 4;92,91,130,131;,
 4;123,132,133,124;,
 4;111,134,135,136;,
 4;137,134,111,110;,
 4;99,138,139,100;,
 4;127,140,141,128;,
 4;142,143,144,145;,
 4;105,104,146,147;,
 4;146,104,103,148;,
 4;148,103,102,149;,
 4;60,63,62,61;,
 4;132,150,151,133;,
 4;134,108,107,135;,
 4;109,108,134,137;,
 4;138,152,153,139;,
 4;140,154,155,141;,
 4;156,157,114,117;,
 4;152,106,109,153;,
 4;139,153,109,137;,
 4;100,139,137,110;,
 4;101,100,110,113;,
 4;158,94,97,159;,
 4;160,161,162,163;,
 4;78,81,164,165;,
 4;82,78,165,60;,
 4;83,82,60,61;,
 4;158,101,113,94;,
 4;112,95,94,113;,
 4;102,143,142,149;,
 4;143,102,105,144;,
 4;145,144,105,147;,
 4;166,122,125,167;,
 4;168,169,90,93;,
 4;81,89,88,164;,
 4;160,126,129,161;,
 4;159,98,101,158;,
 4;170,171,172,173;,
 4;42,133,151,64;,
 4;27,115,114,24;,
 4;39,127,126,36;,
 4;174,80,79,175;,
 4;51,140,127,39;,
 4;175,79,60,176;,
 4;69,154,140,51;,
 4;24,114,157,71;,
 4;36,126,160,74;,
 4;74,160,163,75;,
 4;177,88,80,174;,
 4;178,179,180,181;,
 4;182,183,184,185;,
 4;186,187,188,189;,
 4;190,191,192,193;,
 4;194,195,196,197;,
 4;198,199,200,201;,
 4;202,203,204,205;,
 4;206,207,208,209;,
 4;208,207,210,211;,
 4;211,210,212,213;,
 4;203,214,215,204;,
 4;216,217,207,206;,
 4;207,217,218,210;,
 4;210,218,219,212;,
 4;220,221,222,223;,
 4;224,225,226,227;,
 4;226,225,228,229;,
 4;229,228,230,231;,
 4;232,233,190,193;,
 4;234,235,189,188;,
 4;200,199,182,185;,
 4;236,237,178,181;,
 4;179,238,239,180;,
 4;183,240,241,184;,
 4;242,243,187,186;,
 4;191,244,245,192;,
 4;246,236,181,247;,
 4;180,248,247,181;,
 4;239,249,248,180;,
 4;212,203,202,213;,
 4;219,214,203,212;,
 4;230,221,220,231;,
 4;250,194,197,251;,
 4;223,222,224,227;,
 4;204,215,216,206;,
 4;205,204,206,209;,
 4;252,253,242,186;,
 4;254,252,186,189;,
 4;235,255,254,189;,
 4;256,257,258,259;,
 4;260,261,262,263;,
 4;261,264,265,262;,
 4;264,266,267,265;,
 4;266,260,263,267;,
 4;268,269,270,271;,
 4;272,273,274,275;,
 4;276,277,278,279;,
 4;279,278,280,281;,
 4;281,280,282,283;,
 4;283,282,277,276;,
 4;284,285,286,287;,
 4;287,286,288,289;,
 4;289,288,290,291;,
 4;291,290,285,284;,
 4;292,293,294,295;,
 4;295,294,296,297;,
 4;297,296,298,299;,
 4;299,298,293,292;,
 4;300,301,302,303;,
 4;304,305,306,307;,
 4;308,309,310,311;,
 4;309,312,313,310;,
 4;312,314,315,313;,
 4;314,308,311,315;,
 4;316,317,318,319;,
 4;317,320,321,318;,
 4;320,322,323,321;,
 4;322,316,319,323;;
 
 MeshMaterialList {
  1;
  172;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
  187;
  -0.999997;-0.002518;-0.000000;,
  0.457920;0.888993;0.000001;,
  -0.729975;-0.683473;0.000001;,
  -0.005018;-0.999987;0.000000;,
  -0.639736;-0.575690;0.509234;,
  -0.710074;-0.652765;0.263994;,
  -0.000568;-0.467132;0.884188;,
  -0.003152;-0.955522;0.294903;,
  -0.437787;-0.001104;0.899078;,
  -0.945304;-0.002382;0.326181;,
  0.220362;0.485129;0.846221;,
  0.408573;0.837583;0.362659;,
  -0.999931;-0.011746;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.923997;0.299809;0.237369;,
  0.226559;0.179479;0.957318;,
  -0.442471;-0.003147;0.896778;,
  -0.946960;-0.009182;0.321219;,
  0.877362;0.110778;0.466866;,
  0.647749;-0.611235;0.454768;,
  -0.999997;-0.002521;-0.000000;,
  0.754043;0.562122;0.339761;,
  0.332538;0.264185;0.905331;,
  -0.437787;-0.001104;0.899078;,
  -0.945304;-0.002384;0.326181;,
  -0.002524;0.999997;0.000001;,
  -0.002406;0.953276;0.302092;,
  -0.001163;0.460459;0.887680;,
  0.000000;0.000000;1.000000;,
  -0.185161;-0.411513;0.892397;,
  -0.382488;-0.874054;0.299555;,
  -0.517022;-0.855972;0.000001;,
  -0.414713;-0.164570;0.894947;,
  0.753272;-0.240097;0.612319;,
  0.956099;-0.293045;-0.000000;,
  -0.929069;-0.369906;0.000000;,
  -0.883538;-0.351250;0.309813;,
  0.018030;0.016883;-0.999695;,
  -0.005018;-0.999987;-0.000000;,
  -0.639736;-0.575689;-0.509234;,
  -0.710074;-0.652764;-0.263998;,
  -0.000568;-0.467132;-0.884188;,
  -0.003152;-0.955522;-0.294903;,
  -0.437787;-0.001104;-0.899078;,
  -0.945305;-0.002380;-0.326180;,
  0.220362;0.485129;-0.846221;,
  0.408573;0.837583;-0.362660;,
  0.010135;-0.003632;-0.999942;,
  -0.000066;-0.007155;0.999974;,
  0.923997;0.299809;-0.237369;,
  0.226559;0.179479;-0.957318;,
  -0.442471;-0.003147;-0.896777;,
  -0.946960;-0.009182;-0.321220;,
  0.877361;0.110778;-0.466869;,
  0.647747;-0.611234;-0.454771;,
  0.003272;-0.003666;-0.999988;,
  0.754043;0.562122;-0.339761;,
  0.332538;0.264185;-0.905331;,
  -0.437787;-0.001104;-0.899078;,
  -0.945305;-0.002383;-0.326180;,
  -0.002406;0.953276;-0.302092;,
  -0.001163;0.460458;-0.887681;,
  -0.000000;-0.000000;-1.000000;,
  -0.185148;-0.411503;-0.892404;,
  -0.382488;-0.874053;-0.299557;,
  0.005035;-0.005393;0.999973;,
  0.002181;-0.002444;-0.999995;,
  -0.414703;-0.164559;-0.894954;,
  0.000011;0.000011;-1.000000;,
  0.753269;-0.240097;-0.612323;,
  0.000969;0.004340;0.999990;,
  0.008424;0.006409;0.999944;,
  0.015879;0.008476;-0.999838;,
  -0.883537;-0.351249;-0.309816;,
  0.001830;-0.999998;-0.000000;,
  0.344131;-0.938922;-0.000000;,
  -0.438761;0.898604;0.000000;,
  0.925498;0.378752;0.000000;,
  0.320561;-0.081072;0.943752;,
  0.899148;-0.381487;0.214477;,
  0.899148;-0.381487;-0.214479;,
  0.918758;-0.187272;0.347581;,
  0.997967;0.004285;-0.063584;,
  0.993405;0.114114;0.011132;,
  0.993405;0.114114;-0.011132;,
  0.978514;-0.184501;0.092036;,
  0.978514;-0.184501;-0.092035;,
  0.941290;0.337598;0.000000;,
  0.988017;-0.154347;-0.000000;,
  0.001831;-0.999998;-0.000000;,
  -0.004938;0.999986;0.001708;,
  0.001833;-0.999998;-0.000000;,
  -0.002412;0.999991;0.003416;,
  -0.982630;0.185577;0.000000;,
  0.333781;-0.084320;-0.938872;,
  0.854655;-0.223641;-0.468561;,
  0.960532;-0.257036;-0.106349;,
  0.988016;-0.154349;-0.000000;,
  0.960532;-0.257036;0.106350;,
  0.854657;-0.223642;0.468557;,
  0.333786;-0.084321;0.938870;,
  -0.600582;0.194556;-0.775531;,
  -0.118901;0.992834;-0.011998;,
  0.606737;-0.198089;0.769825;,
  0.678749;-0.203013;-0.705752;,
  0.835832;-0.239263;-0.494104;,
  0.607419;-0.301223;-0.735056;,
  0.775353;-0.340257;-0.532026;,
  0.737969;-0.267137;-0.619709;,
  -0.600584;0.194557;0.775530;,
  -0.118901;0.992834;0.011998;,
  0.606741;-0.198091;-0.769821;,
  0.678744;-0.203008;0.705758;,
  0.835832;-0.239261;0.494104;,
  0.607412;-0.301217;0.735064;,
  0.775353;-0.340254;0.532029;,
  0.737971;-0.267137;0.619706;,
  0.987263;0.159099;0.000000;,
  0.815639;0.578562;0.000000;,
  0.866554;0.440939;0.233789;,
  0.988677;0.080223;0.126816;,
  -0.252819;0.077490;0.964405;,
  -0.252821;0.077490;0.964405;,
  0.423652;-0.875803;0.231274;,
  0.694765;-0.230131;-0.681426;,
  0.694765;-0.230133;-0.681425;,
  -0.002406;0.953276;0.302092;,
  -0.002524;0.999997;0.000001;,
  -0.002064;0.817480;0.575953;,
  0.694766;-0.230134;-0.681425;,
  -0.252817;0.077489;0.964406;,
  0.956099;-0.293045;-0.000000;,
  -0.002524;0.999997;-0.000001;,
  0.457920;0.888993;-0.000001;,
  -0.517021;-0.855973;-0.000002;,
  0.987263;0.159099;0.000000;,
  -0.929070;-0.369905;-0.000002;,
  -0.999931;-0.011746;-0.000000;,
  -0.000066;-0.007155;0.999974;,
  0.001603;-0.005411;0.999984;,
  -0.999997;-0.002520;0.000000;,
  0.866554;0.440939;-0.233790;,
  0.988677;0.080223;-0.126817;,
  -0.252810;0.077487;-0.964408;,
  -0.252812;0.077487;-0.964407;,
  0.423652;-0.875803;-0.231275;,
  0.694770;-0.230133;0.681420;,
  0.694770;-0.230134;0.681419;,
  -0.999997;-0.002516;0.000001;,
  -0.002406;0.953276;-0.302093;,
  -0.002524;0.999997;-0.000001;,
  -0.002064;0.817479;-0.575955;,
  -0.729976;-0.683473;-0.000003;,
  0.010136;-0.003632;0.999942;,
  0.015880;0.008476;0.999838;,
  0.003272;-0.003667;0.999988;,
  0.000047;0.000043;-1.000000;,
  0.694771;-0.230135;0.681419;,
  -0.252809;0.077487;-0.964408;,
  0.956099;-0.293045;-0.000000;,
  0.009984;0.012782;0.999869;,
  0.001937;0.008680;0.999960;,
  0.018030;0.016883;0.999695;,
  -0.003481;-0.999994;0.000000;,
  0.999948;-0.010164;-0.000002;,
  0.999991;-0.004144;-0.000001;,
  0.999999;0.001601;0.000000;,
  0.999946;-0.010438;-0.000002;,
  0.320556;-0.081071;-0.943754;,
  0.605051;-0.153021;-0.781343;,
  -0.279028;-0.960283;0.000000;,
  0.000107;0.999994;0.003416;,
  0.347077;-0.087586;-0.933738;,
  0.347082;-0.087587;0.933736;,
  -0.957097;-0.289768;0.000000;,
  -0.507517;-0.833466;-0.218543;,
  -0.727931;0.271334;0.629678;,
  0.496312;0.838132;0.226297;,
  -0.597119;0.179074;-0.781909;,
  -0.086995;-0.948570;0.304379;,
  0.592985;-0.191091;0.782211;,
  -0.507518;-0.833466;0.218542;,
  -0.727931;0.271334;-0.629678;,
  0.496308;0.838133;-0.226298;,
  -0.597119;0.179074;0.781909;,
  -0.086994;-0.948571;-0.304376;,
  0.592981;-0.191090;-0.782214;;
  172;
  4;13,13,13,13;,
  4;29,30,7,6;,
  4;36,32,16,17;,
  4;33,33,19,19;,
  4;26,27,10,11;,
  4;28,28,28,15;,
  4;25,26,11,1;,
  4;30,31,3,7;,
  4;34,34,117,14;,
  4;35,36,17,12;,
  4;13,13,13,13;,
  4;14,117,118,21;,
  4;15,14,21,22;,
  4;28,28,15,22;,
  4;17,16,23,24;,
  4;12,17,24,20;,
  4;119,120,18,18;,
  4;121,121,122,122;,
  4;123,123,19,19;,
  4;124,125,125,124;,
  4;66,13,13,13;,
  4;21,118,1,11;,
  4;22,21,11,10;,
  4;27,28,22,10;,
  4;24,23,8,9;,
  4;20,24,9,0;,
  4;126,26,25,127;,
  4;126,128,27,26;,
  4;23,28,27,8;,
  4;16,28,28,23;,
  4;32,28,28,16;,
  4;4,5,30,29;,
  4;2,31,30,5;,
  4;47,47,72,72;,
  4;55,66,47,47;,
  4;13,13,66,55;,
  4;28,29,28,32;,
  4;28,28,29,6;,
  4;125,125,129,129;,
  4;18,18,33,33;,
  4;130,121,121,130;,
  4;120,131,34,34;,
  4;13,13,13,13;,
  4;72,72,37,37;,
  4;2,5,36,35;,
  4;5,4,32,36;,
  4;70,48,65,71;,
  4;63,41,42,64;,
  4;73,52,51,67;,
  4;69,54,54,69;,
  4;60,46,45,61;,
  4;62,50,13,68;,
  4;132,133,46,60;,
  4;64,42,38,134;,
  4;34,49,135,34;,
  4;136,137,52,73;,
  4;65,48,138,139;,
  4;49,56,118,135;,
  4;50,57,56,49;,
  4;13,57,50,62;,
  4;52,59,58,51;,
  4;137,140,59,52;,
  4;141,53,53,142;,
  4;143,144,144,143;,
  4;145,54,54,145;,
  4;146,146,147,147;,
  4;139,138,28,28;,
  4;56,46,133,118;,
  4;57,45,46,56;,
  4;61,45,57,13;,
  4;59,44,43,58;,
  4;140,148,44,59;,
  4;149,150,132,60;,
  4;149,60,61,151;,
  4;58,43,61,13;,
  4;51,58,13,62;,
  4;67,51,62,68;,
  4;39,63,64,40;,
  4;152,40,64,134;,
  4;153,154,71,65;,
  4;155,153,65,139;,
  4;28,155,139,28;,
  4;156,67,68,63;,
  4;13,41,63,68;,
  4;147,157,157,147;,
  4;53,69,69,53;,
  4;158,158,143,143;,
  4;142,34,34,159;,
  4;160,161,70,71;,
  4;154,162,160,71;,
  4;152,136,73,40;,
  4;40,73,67,39;,
  4;163,163,163,163;,
  4;118,118,133,1;,
  4;1,133,132,25;,
  4;12,137,136,35;,
  4;164,164,165,165;,
  4;20,140,137,12;,
  4;165,165,166,166;,
  4;0,148,140,20;,
  4;25,132,150,127;,
  4;35,136,152,2;,
  4;2,152,134,31;,
  4;167,167,164,164;,
  4;13,13,13,13;,
  4;90,92,92,90;,
  4;28,28,28,28;,
  4;89,91,91,89;,
  4;97,88,88,97;,
  4;93,76,76,93;,
  4;75,80,79,75;,
  4;99,81,78,100;,
  4;77,77,82,77;,
  4;168,169,95,94;,
  4;80,84,83,79;,
  4;98,85,81,99;,
  4;81,85,86,82;,
  4;82,86,96,95;,
  4;84,87,87,83;,
  4;13,13,13,13;,
  4;170,170,170,170;,
  4;28,28,28,28;,
  4;74,74,89,89;,
  4;28,28,28,28;,
  4;76,76,90,90;,
  4;13,13,13,13;,
  4;13,168,94,13;,
  4;92,171,171,92;,
  4;100,78,28,28;,
  4;91,75,75,91;,
  4;13,13,13,13;,
  4;13,13,13,13;,
  4;94,172,13,13;,
  4;95,80,172,94;,
  4;96,84,80,95;,
  4;28,28,28,28;,
  4;87,97,97,87;,
  4;13,13,13,13;,
  4;79,83,98,99;,
  4;173,79,99,100;,
  4;28,173,100,28;,
  4;28,28,28,28;,
  4;28,28,28,28;,
  4;174,93,93,174;,
  4;101,101,101,101;,
  4;102,102,102,102;,
  4;103,103,103,103;,
  4;175,175,175,175;,
  4;108,108,108,108;,
  4;176,176,176,176;,
  4;105,105,104,104;,
  4;104,104,106,106;,
  4;106,106,107,107;,
  4;107,107,105,105;,
  4;177,177,177,177;,
  4;178,178,178,178;,
  4;179,179,179,179;,
  4;180,180,180,180;,
  4;109,109,109,109;,
  4;110,110,110,110;,
  4;111,111,111,111;,
  4;181,181,181,181;,
  4;116,116,116,116;,
  4;182,182,182,182;,
  4;113,112,112,113;,
  4;112,114,114,112;,
  4;114,115,115,114;,
  4;115,113,113,115;,
  4;183,183,183,183;,
  4;184,184,184,184;,
  4;185,185,185,185;,
  4;186,186,186,186;;
 }
 MeshTextureCoords {
  324;
  0.888021;0.635302;,
  0.842710;0.638725;,
  0.840813;0.608524;,
  0.894581;0.605140;,
  0.928475;0.669683;,
  0.927515;0.677624;,
  0.980092;0.673293;,
  0.973288;0.665593;,
  0.912546;0.661449;,
  0.919685;0.659410;,
  0.917474;0.626019;,
  0.909405;0.623173;,
  0.580284;0.523320;,
  0.604227;0.517774;,
  0.604267;0.583838;,
  0.581035;0.578009;,
  0.923021;0.599410;,
  0.924364;0.607608;,
  0.969177;0.603517;,
  0.974886;0.594675;,
  0.925775;0.625159;,
  0.927746;0.658649;,
  0.975727;0.654130;,
  0.983125;0.619358;,
  0.540552;0.533333;,
  0.551824;0.533528;,
  0.551722;0.589961;,
  0.540450;0.589941;,
  0.603595;0.399500;,
  0.597451;0.399498;,
  0.597436;0.439216;,
  0.603580;0.439219;,
  0.619551;0.428888;,
  0.607981;0.428888;,
  0.607981;0.388845;,
  0.619551;0.388845;,
  0.543449;0.496911;,
  0.552408;0.496911;,
  0.552408;0.468059;,
  0.543449;0.468059;,
  0.839971;0.597131;,
  0.885490;0.593739;,
  0.607981;0.373707;,
  0.619551;0.373707;,
  0.990871;0.617320;,
  0.979638;0.601407;,
  0.973323;0.608226;,
  0.924816;0.612822;,
  0.916651;0.613598;,
  0.908363;0.607442;,
  0.552408;0.456202;,
  0.543449;0.456202;,
  0.577998;0.496601;,
  0.607049;0.485431;,
  0.604214;0.495994;,
  0.580036;0.505290;,
  0.607064;0.511600;,
  0.607113;0.590979;,
  0.579199;0.583975;,
  0.578296;0.518264;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.607981;0.367304;,
  0.619551;0.367304;,
  0.916303;0.608344;,
  0.907923;0.600788;,
  0.552408;0.451186;,
  0.543449;0.451186;,
  0.551853;0.517099;,
  0.540582;0.517079;,
  0.924105;0.664697;,
  0.920269;0.670048;,
  0.543449;0.503392;,
  0.543449;0.509103;,
  0.552408;0.509103;,
  0.552408;0.503392;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.619551;0.442090;,
  0.607981;0.442090;,
  0.843309;0.648348;,
  0.885858;0.645245;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.888021;0.635302;,
  0.894581;0.605140;,
  0.840813;0.608524;,
  0.842710;0.638725;,
  0.872783;0.570858;,
  0.948283;0.568072;,
  0.958991;0.581599;,
  0.870452;0.584083;,
  0.846826;0.555606;,
  0.845039;0.491151;,
  0.858307;0.496655;,
  0.858980;0.552837;,
  0.627487;0.554644;,
  0.629510;0.482655;,
  0.661340;0.457569;,
  0.660031;0.543795;,
  0.870025;0.452549;,
  0.957407;0.449326;,
  0.947032;0.463630;,
  0.871532;0.466415;,
  0.872302;0.495968;,
  0.968977;0.491455;,
  0.953414;0.549076;,
  0.872563;0.552293;,
  0.490487;0.533243;,
  0.490385;0.589850;,
  0.478860;0.589829;,
  0.478962;0.533396;,
  0.563877;0.399485;,
  0.563862;0.439204;,
  0.570006;0.439206;,
  0.570159;0.399487;,
  0.544765;0.428888;,
  0.544765;0.388845;,
  0.556334;0.388845;,
  0.556334;0.428888;,
  0.503653;0.496911;,
  0.503653;0.468059;,
  0.494492;0.468059;,
  0.494492;0.496911;,
  0.885490;0.593739;,
  0.839971;0.597131;,
  0.544765;0.373707;,
  0.556594;0.373707;,
  0.953556;0.471902;,
  0.964761;0.461042;,
  0.982147;0.488741;,
  0.871815;0.475199;,
  0.844722;0.464684;,
  0.858057;0.475757;,
  0.503653;0.456202;,
  0.494492;0.456202;,
  0.623552;0.590707;,
  0.626820;0.578377;,
  0.659599;0.572221;,
  0.662938;0.583310;,
  0.665029;0.445550;,
  0.663456;0.549154;,
  0.626785;0.475692;,
  0.624354;0.562190;,
  0.544765;0.367304;,
  0.556594;0.367304;,
  0.844588;0.453488;,
  0.857951;0.466917;,
  0.503653;0.451186;,
  0.494492;0.451186;,
  0.478992;0.516968;,
  0.490517;0.516989;,
  0.865910;0.562101;,
  0.858992;0.570723;,
  0.503653;0.503392;,
  0.494492;0.503392;,
  0.494492;0.509103;,
  0.503653;0.509103;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.544765;0.442090;,
  0.556334;0.442090;,
  0.843309;0.648348;,
  0.885858;0.645245;,
  0.839726;0.598125;,
  0.842399;0.647987;,
  0.888208;0.645077;,
  0.885534;0.595216;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.448183;0.389727;,
  0.486378;0.386943;,
  0.485709;0.391063;,
  0.447932;0.393109;,
  0.370412;0.540995;,
  0.426996;0.540995;,
  0.426964;0.577108;,
  0.370412;0.577108;,
  0.485709;0.391063;,
  0.486399;0.387182;,
  0.448183;0.389727;,
  0.447932;0.393109;,
  0.370202;0.577108;,
  0.417638;0.577108;,
  0.417638;0.540995;,
  0.370202;0.540995;,
  0.441628;0.477644;,
  0.441612;0.473171;,
  0.457953;0.472839;,
  0.457969;0.477311;,
  0.309161;0.540995;,
  0.317739;0.540995;,
  0.317739;0.577108;,
  0.309161;0.577108;,
  0.427881;0.526485;,
  0.431571;0.521508;,
  0.469120;0.521586;,
  0.472718;0.526577;,
  0.469219;0.469946;,
  0.469234;0.462471;,
  0.472841;0.456497;,
  0.472826;0.465359;,
  0.431685;0.462393;,
  0.428004;0.456405;,
  0.431670;0.469868;,
  0.427989;0.465266;,
  0.433619;0.518119;,
  0.467080;0.518188;,
  0.467175;0.471920;,
  0.467189;0.465223;,
  0.433727;0.465154;,
  0.433714;0.471851;,
  0.403853;0.518921;,
  0.413698;0.516882;,
  0.414453;0.524311;,
  0.404608;0.526349;,
  0.422571;0.470804;,
  0.423746;0.463059;,
  0.403650;0.460519;,
  0.403771;0.468843;,
  0.422992;0.455630;,
  0.402895;0.453090;,
  0.421817;0.463375;,
  0.403016;0.461414;,
  0.338567;0.540995;,
  0.338567;0.577108;,
  0.412609;0.392209;,
  0.406816;0.399699;,
  0.406816;0.399699;,
  0.412609;0.392209;,
  0.513905;0.385265;,
  0.512565;0.392280;,
  0.467723;0.540995;,
  0.467723;0.577108;,
  0.512565;0.392280;,
  0.513905;0.385265;,
  0.451061;0.577108;,
  0.451061;0.540995;,
  0.427329;0.445836;,
  0.448695;0.444404;,
  0.480732;0.442257;,
  0.503305;0.440744;,
  0.441733;0.508544;,
  0.458074;0.508211;,
  0.480732;0.442257;,
  0.503305;0.440744;,
  0.448695;0.444404;,
  0.427329;0.445836;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.615570;0.454391;,
  0.649832;0.427985;,
  0.660274;0.430985;,
  0.625881;0.458534;,
  0.652911;0.426302;,
  0.663383;0.429044;,
  0.608800;0.456438;,
  0.619112;0.460582;,
  0.582182;0.587473;,
  0.582173;0.511047;,
  0.604741;0.488565;,
  0.606085;0.578398;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.578929;0.596105;,
  0.582198;0.587640;,
  0.606100;0.578566;,
  0.608406;0.584963;,
  0.604757;0.488733;,
  0.608247;0.481203;,
  0.582189;0.511214;,
  0.578708;0.509010;,
  0.622140;0.459392;,
  0.614889;0.457024;,
  0.606152;0.458508;,
  0.613403;0.460877;,
  0.650956;0.426634;,
  0.658206;0.429002;,
  0.648399;0.428878;,
  0.655650;0.431246;,
  0.619904;0.462103;,
  0.618261;0.450040;,
  0.655580;0.422184;,
  0.655991;0.434092;,
  0.657569;0.422099;,
  0.657702;0.433972;,
  0.613591;0.459127;,
  0.615235;0.471190;,
  0.581569;0.588583;,
  0.604200;0.579105;,
  0.605573;0.486996;,
  0.583567;0.510310;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.578755;0.597255;,
  0.606715;0.585595;,
  0.604636;0.579097;,
  0.582006;0.588575;,
  0.609562;0.479214;,
  0.606009;0.486988;,
  0.580857;0.508048;,
  0.584003;0.510302;,
  0.618478;0.452784;,
  0.615524;0.458541;,
  0.615947;0.465152;,
  0.618900;0.459396;,
  0.652882;0.427003;,
  0.653305;0.433614;,
  0.650133;0.429994;,
  0.650555;0.436606;;
 }
}
