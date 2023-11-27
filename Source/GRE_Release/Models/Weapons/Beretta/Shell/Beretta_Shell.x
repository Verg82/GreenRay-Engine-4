xof 0303txt 0032
template Frame {
 <3d82ab46-62da-11cf-ab39-0020af71e433>
 [...]
}

template Matrix4x4 {
 <f6f23f45-7686-11cf-8f52-0040333594a3>
 array FLOAT matrix[16];
}

template FrameTransformMatrix {
 <f6f23f41-7686-11cf-8f52-0040333594a3>
 Matrix4x4 frameMatrix;
}

template Vector {
 <3d82ab5e-62da-11cf-ab39-0020af71e433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template MeshFace {
 <3d82ab5f-62da-11cf-ab39-0020af71e433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template Mesh {
 <3d82ab44-62da-11cf-ab39-0020af71e433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

template MeshNormals {
 <f6f23f43-7686-11cf-8f52-0040333594a3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template Coords2d {
 <f6f23f44-7686-11cf-8f52-0040333594a3>
 FLOAT u;
 FLOAT v;
}

template MeshTextureCoords {
 <f6f23f40-7686-11cf-8f52-0040333594a3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}


Frame Cylinder01 {
 

 FrameTransformMatrix {
  -0.001437,-0.000000,-0.000000,0.000000,0.000000,0.000000,-0.001603,0.000000,-0.000000,-0.001450,0.000000,0.000000,0.000000,-0.000000,0.000000,1.000000;;
 }

 Frame {
  

  FrameTransformMatrix {
   1.000000,0.000000,0.000000,0.000000,-0.000000,1.000000,0.000000,0.000000,-0.000000,0.000000,1.000000,0.000000,-0.000004,-136.680191,-0.000002,1.000000;;
  }

  Mesh  {
   154;
   76.840408;40.986221;0.000000;,
   72.206375;40.986221;26.280966;,
   58.863163;40.986221;49.392059;,
   38.420200;40.986221;66.545738;,
   13.343195;40.986221;75.673027;,
   -13.343200;40.986221;75.673019;,
   -38.420200;40.986221;66.545738;,
   -58.863159;40.986221;49.392059;,
   -72.206345;40.986221;26.280972;,
   -76.840393;40.986221;0.000012;,
   -72.206345;40.986221;-26.280949;,
   -58.863171;40.986221;-49.392040;,
   -38.420219;40.986221;-66.545723;,
   -13.343223;40.986221;-75.673019;,
   13.343163;40.986221;-75.673027;,
   38.420166;40.986221;-66.545761;,
   58.863132;40.986221;-49.392094;,
   72.206345;40.986221;-26.281015;,
   76.840401;272.656799;0.000000;,
   72.206360;272.656799;26.280966;,
   58.863159;272.656799;49.392059;,
   38.420197;272.656799;66.545738;,
   13.343189;272.656799;75.673027;,
   -13.343205;272.656799;75.673019;,
   -38.420204;272.656799;66.545738;,
   -58.863163;272.656799;49.392059;,
   -72.206352;272.656799;26.280972;,
   -76.840401;272.656799;0.000012;,
   -72.206360;272.656799;-26.280949;,
   -58.863174;272.656799;-49.392040;,
   -38.420223;272.656799;-66.545723;,
   -13.343228;272.656799;-75.673019;,
   13.343158;272.656799;-75.673027;,
   38.420162;272.656799;-66.545761;,
   58.863129;272.656799;-49.392094;,
   72.206337;272.656799;-26.281015;,
   64.908577;25.585340;0.000000;,
   60.994122;25.585340;22.200043;,
   49.722855;25.585340;41.722431;,
   32.454288;25.585340;56.212482;,
   11.271255;25.585340;63.922482;,
   -11.271260;25.585340;63.922470;,
   -32.454288;25.585339;56.212482;,
   -49.722855;25.585340;41.722431;,
   -60.994106;25.585340;22.200047;,
   -64.908577;25.585340;0.000010;,
   -60.994106;25.585340;-22.200027;,
   -49.722862;25.585340;-41.722416;,
   -32.454304;25.585339;-56.212456;,
   -11.271279;25.585340;-63.922470;,
   11.271229;25.585340;-63.922470;,
   32.454262;25.585340;-56.212498;,
   49.722832;25.585340;-41.722462;,
   60.994095;25.585340;-22.200085;,
   64.908577;13.836593;0.000000;,
   60.994122;13.836593;22.200043;,
   49.722855;13.836592;41.722431;,
   32.454288;13.836591;56.212482;,
   11.271255;13.836593;63.922482;,
   -11.271260;13.836591;63.922470;,
   -32.454288;13.836591;56.212482;,
   -49.722855;13.836592;41.722431;,
   -60.994106;13.836591;22.200047;,
   -64.908577;13.836591;0.000010;,
   -60.994106;13.836591;-22.200027;,
   -49.722862;13.836592;-41.722416;,
   -32.454304;13.836591;-56.212456;,
   -11.271279;13.836591;-63.922470;,
   11.271229;13.836593;-63.922470;,
   32.454262;13.836591;-56.212498;,
   49.722832;13.836592;-41.722462;,
   60.994095;13.836593;-22.200085;,
   76.840401;13.836592;0.000000;,
   72.206367;13.836592;26.280966;,
   58.863163;13.836592;49.392059;,
   38.420204;13.836590;66.545746;,
   13.343195;13.836592;75.673035;,
   -13.343199;13.836590;75.673019;,
   -38.420197;13.836590;66.545746;,
   -58.863155;13.836592;49.392059;,
   -72.206352;13.836590;26.280970;,
   -76.840401;13.836590;0.000012;,
   -72.206352;13.836590;-26.280949;,
   -58.863163;13.836592;-49.392040;,
   -38.420212;13.836590;-66.545708;,
   -13.343222;13.836590;-75.673019;,
   13.343163;13.836592;-75.673019;,
   38.420170;13.836590;-66.545761;,
   58.863136;13.836592;-49.392094;,
   72.206337;13.836592;-26.281015;,
   76.840401;0.703581;0.000000;,
   72.206367;0.703581;26.280966;,
   58.863163;0.703581;49.392059;,
   38.420204;0.703579;66.545746;,
   13.343197;0.703581;75.673035;,
   -13.343197;0.703579;75.673019;,
   -38.420197;0.703579;66.545746;,
   -58.863155;0.703581;49.392059;,
   -72.206352;0.703579;26.280970;,
   -76.840401;0.703579;0.000012;,
   -72.206352;0.703579;-26.280949;,
   -58.863163;0.703581;-49.392040;,
   -38.420212;0.703579;-66.545708;,
   -13.343220;0.703579;-75.673019;,
   13.343165;0.703581;-75.673019;,
   38.420170;0.703579;-66.545761;,
   58.863136;0.703581;-49.392094;,
   72.206337;0.703581;-26.281015;,
   -0.000007;0.703583;-0.000000;,
   69.600365;272.656799;-0.000001;,
   65.402954;272.656799;23.804728;,
   53.316963;272.656799;44.738251;,
   34.800175;272.656799;60.275681;,
   12.085970;272.656799;68.542984;,
   -12.085986;272.656799;68.542976;,
   -34.800190;272.656799;60.275681;,
   -53.316978;272.656799;44.738251;,
   -65.402939;272.656799;23.804731;,
   -69.600357;272.656799;0.000010;,
   -65.402946;272.656799;-23.804710;,
   -53.316990;272.656799;-44.738235;,
   -34.800205;272.656799;-60.275669;,
   -12.086007;272.656799;-68.542976;,
   12.085941;272.656799;-68.542984;,
   34.800144;272.656799;-60.275703;,
   53.316936;272.656799;-44.738285;,
   65.402939;272.656799;-23.804773;,
   69.600372;48.898350;0.000000;,
   65.402954;48.898350;23.804728;,
   53.316978;48.898350;44.738251;,
   34.800182;48.898350;60.275681;,
   12.085981;48.898350;68.542984;,
   -12.085975;48.898350;68.542984;,
   -34.800179;48.898350;60.275681;,
   -53.316963;48.898350;44.738251;,
   -65.402939;48.898350;23.804731;,
   -69.600357;48.898350;0.000011;,
   -65.402939;48.898350;-23.804710;,
   -53.316978;48.898350;-44.738235;,
   -34.800198;48.898350;-60.275669;,
   -12.085996;48.898350;-68.542969;,
   12.085953;48.898350;-68.542984;,
   34.800156;48.898350;-60.275703;,
   53.316952;48.898350;-44.738285;,
   65.402939;48.898350;-23.804773;,
   -0.000002;48.898373;-0.000007;,
   -13.343200;40.986221;75.673019;,
   -13.343205;272.656799;75.673019;,
   -11.271260;25.585340;63.922470;,
   -11.271260;13.836591;63.922470;,
   -13.343199;13.836590;75.673019;,
   -13.343197;0.703579;75.673019;,
   -12.085986;272.656799;68.542976;,
   -12.085975;48.898350;68.542984;;
   288;
   3;0,19,1;,
   3;19,0,18;,
   3;1,20,2;,
   3;20,1,19;,
   3;2,21,3;,
   3;21,2,20;,
   3;3,22,4;,
   3;22,3,21;,
   3;4,23,5;,
   3;23,4,22;,
   3;146,24,6;,
   3;24,146,147;,
   3;6,25,7;,
   3;25,6,24;,
   3;7,26,8;,
   3;26,7,25;,
   3;8,27,9;,
   3;27,8,26;,
   3;9,28,10;,
   3;28,9,27;,
   3;10,29,11;,
   3;29,10,28;,
   3;11,30,12;,
   3;30,11,29;,
   3;12,31,13;,
   3;31,12,30;,
   3;13,32,14;,
   3;32,13,31;,
   3;14,33,15;,
   3;33,14,32;,
   3;15,34,16;,
   3;34,15,33;,
   3;16,35,17;,
   3;35,16,34;,
   3;17,18,0;,
   3;18,17,35;,
   3;1,36,0;,
   3;36,1,37;,
   3;2,37,1;,
   3;37,2,38;,
   3;3,38,2;,
   3;38,3,39;,
   3;4,39,3;,
   3;39,4,40;,
   3;5,40,4;,
   3;40,5,41;,
   3;6,148,146;,
   3;148,6,42;,
   3;7,42,6;,
   3;42,7,43;,
   3;8,43,7;,
   3;43,8,44;,
   3;9,44,8;,
   3;44,9,45;,
   3;10,45,9;,
   3;45,10,46;,
   3;11,46,10;,
   3;46,11,47;,
   3;12,47,11;,
   3;47,12,48;,
   3;13,48,12;,
   3;48,13,49;,
   3;14,49,13;,
   3;49,14,50;,
   3;15,50,14;,
   3;50,15,51;,
   3;16,51,15;,
   3;51,16,52;,
   3;17,52,16;,
   3;52,17,53;,
   3;0,53,17;,
   3;53,0,36;,
   3;37,54,36;,
   3;54,37,55;,
   3;38,55,37;,
   3;55,38,56;,
   3;39,56,38;,
   3;56,39,57;,
   3;40,57,39;,
   3;57,40,58;,
   3;41,58,40;,
   3;58,41,59;,
   3;42,149,148;,
   3;149,42,60;,
   3;43,60,42;,
   3;60,43,61;,
   3;44,61,43;,
   3;61,44,62;,
   3;45,62,44;,
   3;62,45,63;,
   3;46,63,45;,
   3;63,46,64;,
   3;47,64,46;,
   3;64,47,65;,
   3;48,65,47;,
   3;65,48,66;,
   3;49,66,48;,
   3;66,49,67;,
   3;50,67,49;,
   3;67,50,68;,
   3;51,68,50;,
   3;68,51,69;,
   3;52,69,51;,
   3;69,52,70;,
   3;53,70,52;,
   3;70,53,71;,
   3;36,71,53;,
   3;71,36,54;,
   3;55,72,54;,
   3;72,55,73;,
   3;56,73,55;,
   3;73,56,74;,
   3;57,74,56;,
   3;74,57,75;,
   3;58,75,57;,
   3;75,58,76;,
   3;59,76,58;,
   3;76,59,77;,
   3;60,150,149;,
   3;150,60,78;,
   3;61,78,60;,
   3;78,61,79;,
   3;62,79,61;,
   3;79,62,80;,
   3;63,80,62;,
   3;80,63,81;,
   3;64,81,63;,
   3;81,64,82;,
   3;65,82,64;,
   3;82,65,83;,
   3;66,83,65;,
   3;83,66,84;,
   3;67,84,66;,
   3;84,67,85;,
   3;68,85,67;,
   3;85,68,86;,
   3;69,86,68;,
   3;86,69,87;,
   3;70,87,69;,
   3;87,70,88;,
   3;71,88,70;,
   3;88,71,89;,
   3;54,89,71;,
   3;89,54,72;,
   3;73,90,72;,
   3;90,73,91;,
   3;74,91,73;,
   3;91,74,92;,
   3;75,92,74;,
   3;92,75,93;,
   3;76,93,75;,
   3;93,76,94;,
   3;77,94,76;,
   3;94,77,95;,
   3;78,151,150;,
   3;151,78,96;,
   3;79,96,78;,
   3;96,79,97;,
   3;80,97,79;,
   3;97,80,98;,
   3;81,98,80;,
   3;98,81,99;,
   3;82,99,81;,
   3;99,82,100;,
   3;83,100,82;,
   3;100,83,101;,
   3;84,101,83;,
   3;101,84,102;,
   3;85,102,84;,
   3;102,85,103;,
   3;86,103,85;,
   3;103,86,104;,
   3;87,104,86;,
   3;104,87,105;,
   3;88,105,87;,
   3;105,88,106;,
   3;89,106,88;,
   3;106,89,107;,
   3;72,107,89;,
   3;107,72,90;,
   3;91,108,90;,
   3;92,108,91;,
   3;93,108,92;,
   3;94,108,93;,
   3;95,108,94;,
   3;96,108,151;,
   3;97,108,96;,
   3;98,108,97;,
   3;99,108,98;,
   3;100,108,99;,
   3;101,108,100;,
   3;102,108,101;,
   3;103,108,102;,
   3;104,108,103;,
   3;105,108,104;,
   3;106,108,105;,
   3;107,108,106;,
   3;90,108,107;,
   3;18,110,19;,
   3;110,18,109;,
   3;19,111,20;,
   3;111,19,110;,
   3;20,112,21;,
   3;112,20,111;,
   3;21,113,22;,
   3;113,21,112;,
   3;22,114,23;,
   3;114,22,113;,
   3;147,115,24;,
   3;115,147,152;,
   3;24,116,25;,
   3;116,24,115;,
   3;25,117,26;,
   3;117,25,116;,
   3;26,118,27;,
   3;118,26,117;,
   3;27,119,28;,
   3;119,27,118;,
   3;28,120,29;,
   3;120,28,119;,
   3;29,121,30;,
   3;121,29,120;,
   3;30,122,31;,
   3;122,30,121;,
   3;31,123,32;,
   3;123,31,122;,
   3;32,124,33;,
   3;124,32,123;,
   3;33,125,34;,
   3;125,33,124;,
   3;34,126,35;,
   3;126,34,125;,
   3;35,109,18;,
   3;109,35,126;,
   3;109,128,110;,
   3;128,109,127;,
   3;110,129,111;,
   3;129,110,128;,
   3;111,130,112;,
   3;130,111,129;,
   3;112,131,113;,
   3;131,112,130;,
   3;113,132,114;,
   3;132,113,131;,
   3;152,133,115;,
   3;133,152,153;,
   3;115,134,116;,
   3;134,115,133;,
   3;116,135,117;,
   3;135,116,134;,
   3;117,136,118;,
   3;136,117,135;,
   3;118,137,119;,
   3;137,118,136;,
   3;119,138,120;,
   3;138,119,137;,
   3;120,139,121;,
   3;139,120,138;,
   3;121,140,122;,
   3;140,121,139;,
   3;122,141,123;,
   3;141,122,140;,
   3;123,142,124;,
   3;142,123,141;,
   3;124,143,125;,
   3;143,124,142;,
   3;125,144,126;,
   3;144,125,143;,
   3;126,127,109;,
   3;127,126,144;,
   3;127,145,128;,
   3;128,145,129;,
   3;129,145,130;,
   3;130,145,131;,
   3;131,145,132;,
   3;153,145,133;,
   3;133,145,134;,
   3;134,145,135;,
   3;135,145,136;,
   3;136,145,137;,
   3;137,145,138;,
   3;138,145,139;,
   3;139,145,140;,
   3;140,145,141;,
   3;141,145,142;,
   3;142,145,143;,
   3;143,145,144;,
   3;144,145,127;;

   MeshNormals  {
    243;
    1.000000;0.000000;-0.000000;,
    0.939693;0.000000;0.342020;,
    0.939693;0.000000;0.342020;,
    1.000000;0.000000;-0.000000;,
    0.766044;0.000000;0.642788;,
    0.766044;0.000000;0.642788;,
    0.500000;0.000000;0.866025;,
    0.500000;0.000000;0.866025;,
    0.173648;0.000000;0.984808;,
    0.173648;0.000000;0.984808;,
    -0.173648;-0.000000;0.984808;,
    -0.173648;-0.000000;0.984808;,
    -0.500000;-0.000000;0.866025;,
    -0.500000;-0.000000;0.866025;,
    -0.766044;-0.000000;0.642788;,
    -0.766045;-0.000000;0.642787;,
    -0.939693;-0.000000;0.342020;,
    -0.939693;-0.000000;0.342020;,
    -1.000000;-0.000000;0.000000;,
    -1.000000;-0.000000;0.000000;,
    -0.939693;-0.000000;-0.342020;,
    -0.939693;-0.000000;-0.342020;,
    -0.766045;-0.000000;-0.642787;,
    -0.766045;-0.000000;-0.642787;,
    -0.500000;-0.000000;-0.866025;,
    -0.500000;-0.000000;-0.866025;,
    -0.173649;-0.000000;-0.984808;,
    -0.173649;-0.000000;-0.984808;,
    0.173648;0.000000;-0.984808;,
    0.173648;0.000000;-0.984808;,
    0.500000;0.000000;-0.866026;,
    0.500000;0.000000;-0.866026;,
    0.766044;0.000000;-0.642788;,
    0.766044;0.000000;-0.642788;,
    0.939692;0.000000;-0.342021;,
    0.939692;0.000000;-0.342021;,
    0.742837;-0.612448;0.270371;,
    0.790511;-0.612448;-0.000000;,
    0.790511;-0.612448;-0.000000;,
    0.742837;-0.612448;0.270371;,
    0.605566;-0.612448;0.508131;,
    0.605566;-0.612448;0.508131;,
    0.395256;-0.612448;0.684603;,
    0.395256;-0.612448;0.684603;,
    0.137271;-0.612448;0.778501;,
    0.137271;-0.612448;0.778501;,
    -0.137271;-0.612448;0.778501;,
    -0.137271;-0.612448;0.778501;,
    -0.395256;-0.612448;0.684603;,
    -0.395255;-0.612448;0.684603;,
    -0.605567;-0.612448;0.508131;,
    -0.605567;-0.612448;0.508131;,
    -0.742837;-0.612448;0.270371;,
    -0.742837;-0.612448;0.270371;,
    -0.790511;-0.612448;0.000000;,
    -0.790511;-0.612448;-0.000000;,
    -0.742838;-0.612448;-0.270371;,
    -0.742838;-0.612448;-0.270371;,
    -0.605567;-0.612448;-0.508130;,
    -0.605567;-0.612448;-0.508130;,
    -0.395256;-0.612448;-0.684602;,
    -0.395256;-0.612448;-0.684602;,
    -0.137271;-0.612448;-0.778501;,
    -0.137271;-0.612448;-0.778501;,
    0.137270;-0.612448;-0.778501;,
    0.137271;-0.612448;-0.778501;,
    0.395255;-0.612448;-0.684603;,
    0.395255;-0.612448;-0.684603;,
    0.605566;-0.612448;-0.508131;,
    0.605566;-0.612448;-0.508131;,
    0.742837;-0.612448;-0.270371;,
    0.742837;-0.612448;-0.270371;,
    0.939693;0.000000;0.342020;,
    1.000000;0.000000;-0.000000;,
    1.000000;0.000000;-0.000000;,
    0.766044;0.000000;0.642788;,
    0.766044;0.000000;0.642788;,
    0.500000;0.000000;0.866025;,
    0.173648;0.000000;0.984808;,
    0.173648;0.000000;0.984808;,
    -0.173648;0.000000;0.984808;,
    -0.173648;0.000000;0.984808;,
    -0.500000;0.000000;0.866025;,
    -0.500000;0.000000;0.866025;,
    -0.766044;0.000000;0.642788;,
    -0.939693;0.000000;0.342020;,
    -0.939693;0.000000;0.342020;,
    -1.000000;0.000000;-0.000000;,
    -1.000000;0.000000;0.000000;,
    -0.939693;0.000000;-0.342020;,
    -0.939693;0.000000;-0.342020;,
    -0.766045;0.000000;-0.642787;,
    -0.500000;0.000000;-0.866025;,
    -0.173649;0.000000;-0.984808;,
    -0.173649;0.000000;-0.984808;,
    0.173648;0.000000;-0.984808;,
    0.173648;0.000000;-0.984808;,
    0.499999;0.000000;-0.866026;,
    0.499999;0.000000;-0.866026;,
    0.766044;0.000000;-0.642788;,
    0.766044;0.000000;-0.642788;,
    0.939692;0.000000;-0.342021;,
    0.939692;0.000000;-0.342021;,
    0.000000;1.000000;0.000000;,
    0.000000;1.000000;-0.000000;,
    0.000000;1.000000;-0.000000;,
    0.000000;1.000000;0.000000;,
    -0.000000;1.000000;0.000000;,
    -0.000000;1.000000;0.000000;,
    0.000000;1.000000;0.000000;,
    0.000000;1.000000;0.000000;,
    0.000000;1.000000;0.000000;,
    0.000000;1.000000;0.000000;,
    -0.000000;1.000000;0.000000;,
    -0.000000;1.000000;0.000000;,
    -0.000000;1.000000;0.000000;,
    -0.000000;1.000000;0.000000;,
    0.000000;1.000000;0.000000;,
    -0.000000;1.000000;0.000000;,
    -0.000000;1.000000;-0.000000;,
    -0.000000;1.000000;-0.000000;,
    -0.000000;1.000000;-0.000000;,
    -0.000000;1.000000;0.000000;,
    -0.000000;1.000000;-0.000000;,
    -0.000000;1.000000;0.000000;,
    -0.000000;1.000000;0.000000;,
    -0.000000;1.000000;0.000000;,
    -0.000000;1.000000;-0.000000;,
    -0.000000;1.000000;-0.000000;,
    -0.000000;1.000000;-0.000000;,
    -0.000000;1.000000;-0.000000;,
    0.000000;1.000000;-0.000000;,
    0.000000;1.000000;-0.000000;,
    0.000000;1.000000;-0.000000;,
    0.000000;1.000000;-0.000000;,
    -0.000000;1.000000;-0.000000;,
    -0.000000;1.000000;-0.000000;,
    0.000000;1.000000;-0.000000;,
    0.000000;1.000000;-0.000000;,
    0.939693;0.000000;0.342020;,
    1.000000;0.000000;-0.000000;,
    0.766044;0.000000;0.642788;,
    0.766044;0.000000;0.642788;,
    0.500000;0.000000;0.866025;,
    0.500000;0.000000;0.866025;,
    0.173648;0.000000;0.984808;,
    0.173648;0.000000;0.984808;,
    -0.173648;-0.000000;0.984808;,
    -0.173648;-0.000000;0.984808;,
    -0.500000;-0.000000;0.866025;,
    -0.766044;0.000000;0.642788;,
    -0.766044;0.000000;0.642788;,
    -0.939693;0.000000;0.342020;,
    -0.939693;0.000000;0.342020;,
    -1.000000;0.000000;0.000000;,
    -1.000000;0.000000;0.000000;,
    -0.939693;0.000000;-0.342020;,
    -0.939693;0.000000;-0.342020;,
    -0.766044;0.000000;-0.642788;,
    -0.766044;0.000000;-0.642788;,
    -0.500000;0.000000;-0.866025;,
    -0.500000;-0.000000;-0.866025;,
    -0.173649;-0.000000;-0.984808;,
    -0.173649;-0.000000;-0.984808;,
    0.173648;0.000000;-0.984808;,
    0.173648;0.000000;-0.984808;,
    0.499999;0.000000;-0.866026;,
    0.766044;0.000000;-0.642788;,
    -0.000000;-1.000000;-0.000000;,
    0.000000;-1.000000;0.000000;,
    -0.000000;-1.000000;0.000000;,
    0.000000;-1.000000;-0.000000;,
    -0.000000;-1.000000;-0.000000;,
    -0.000000;-1.000000;-0.000000;,
    0.000000;-1.000000;-0.000000;,
    -0.000000;-1.000000;-0.000000;,
    0.000000;-1.000000;-0.000000;,
    0.000000;-1.000000;0.000000;,
    0.000000;-1.000000;-0.000000;,
    0.000000;-1.000000;-0.000000;,
    0.000000;-1.000000;0.000000;,
    -0.000000;-1.000000;0.000000;,
    0.000000;-1.000000;0.000000;,
    -0.000000;-1.000000;0.000000;,
    -0.000000;-1.000000;0.000000;,
    0.000000;-1.000000;0.000000;,
    -0.000000;-1.000000;0.000000;,
    0.000000;1.000000;0.000000;,
    -1.000000;-0.000000;0.000000;,
    -0.939693;-0.000000;-0.342020;,
    -0.939693;-0.000000;-0.342020;,
    -1.000000;-0.000000;0.000000;,
    -0.766044;-0.000000;-0.642788;,
    -0.766044;-0.000000;-0.642788;,
    -0.500000;-0.000000;-0.866025;,
    -0.500000;-0.000000;-0.866025;,
    -0.173648;-0.000000;-0.984808;,
    -0.173648;-0.000000;-0.984808;,
    0.173648;-0.000000;-0.984808;,
    0.173648;-0.000000;-0.984808;,
    0.500000;0.000000;-0.866025;,
    0.500000;0.000000;-0.866025;,
    0.766044;0.000000;-0.642788;,
    0.766045;0.000000;-0.642787;,
    0.939693;0.000000;-0.342020;,
    0.939693;-0.000000;-0.342020;,
    1.000000;0.000000;-0.000000;,
    1.000000;0.000000;-0.000000;,
    0.939693;0.000000;0.342020;,
    0.939693;0.000000;0.342020;,
    0.766045;0.000000;0.642787;,
    0.766045;0.000000;0.642787;,
    0.500000;0.000000;0.866025;,
    0.500000;0.000000;0.866025;,
    0.173649;0.000000;0.984808;,
    0.173649;0.000000;0.984808;,
    -0.173648;0.000000;0.984808;,
    -0.173648;-0.000000;0.984808;,
    -0.500000;-0.000000;0.866026;,
    -0.500000;-0.000000;0.866026;,
    -0.766044;-0.000000;0.642788;,
    -0.766044;-0.000000;0.642788;,
    -0.939692;-0.000000;0.342021;,
    -0.939692;-0.000000;0.342021;,
    0.000000;1.000000;-0.000000;,
    -0.000000;1.000000;-0.000000;,
    0.000000;1.000000;0.000000;,
    0.000000;1.000000;0.000000;,
    0.000000;1.000000;0.000000;,
    0.000000;1.000000;0.000000;,
    -0.000000;1.000000;0.000000;,
    -0.000000;1.000000;0.000000;,
    -0.000000;1.000000;0.000000;,
    -0.000000;1.000000;0.000000;,
    -0.000000;1.000000;0.000000;,
    -0.000000;1.000000;-0.000000;,
    -0.000000;1.000000;-0.000000;,
    -0.000000;1.000000;-0.000000;,
    -0.000000;1.000000;-0.000000;,
    0.000000;1.000000;-0.000000;,
    0.000000;1.000000;-0.000000;,
    0.000000;1.000000;-0.000000;,
    0.000000;1.000000;-0.000000;;
    288;
    3;0,1,2;,
    3;1,0,3;,
    3;2,4,5;,
    3;4,2,1;,
    3;5,6,7;,
    3;6,5,4;,
    3;7,8,9;,
    3;8,7,6;,
    3;9,10,11;,
    3;10,9,8;,
    3;11,12,13;,
    3;12,11,10;,
    3;13,14,15;,
    3;14,13,12;,
    3;15,16,17;,
    3;16,15,14;,
    3;17,18,19;,
    3;18,17,16;,
    3;19,20,21;,
    3;20,19,18;,
    3;21,22,23;,
    3;22,21,20;,
    3;23,24,25;,
    3;24,23,22;,
    3;25,26,27;,
    3;26,25,24;,
    3;27,28,29;,
    3;28,27,26;,
    3;29,30,31;,
    3;30,29,28;,
    3;31,32,33;,
    3;32,31,30;,
    3;33,34,35;,
    3;34,33,32;,
    3;35,3,0;,
    3;3,35,34;,
    3;36,37,38;,
    3;37,36,39;,
    3;40,39,36;,
    3;39,40,41;,
    3;42,41,40;,
    3;41,42,43;,
    3;44,43,42;,
    3;43,44,45;,
    3;46,45,44;,
    3;45,46,47;,
    3;48,47,46;,
    3;47,48,49;,
    3;50,49,48;,
    3;49,50,51;,
    3;52,51,50;,
    3;51,52,53;,
    3;54,53,52;,
    3;53,54,55;,
    3;56,55,54;,
    3;55,56,57;,
    3;58,57,56;,
    3;57,58,59;,
    3;60,59,58;,
    3;59,60,61;,
    3;62,61,60;,
    3;61,62,63;,
    3;64,63,62;,
    3;63,64,65;,
    3;66,65,64;,
    3;65,66,67;,
    3;68,67,66;,
    3;67,68,69;,
    3;70,69,68;,
    3;69,70,71;,
    3;38,71,70;,
    3;71,38,37;,
    3;72,73,74;,
    3;73,72,72;,
    3;75,72,72;,
    3;72,75,76;,
    3;77,76,75;,
    3;76,77,77;,
    3;78,77,77;,
    3;77,78,79;,
    3;80,79,78;,
    3;79,80,81;,
    3;82,81,80;,
    3;81,82,83;,
    3;84,83,82;,
    3;83,84,84;,
    3;85,84,84;,
    3;84,85,86;,
    3;87,86,85;,
    3;86,87,88;,
    3;89,88,87;,
    3;88,89,90;,
    3;91,90,89;,
    3;90,91,91;,
    3;92,91,91;,
    3;91,92,92;,
    3;93,92,92;,
    3;92,93,94;,
    3;95,94,93;,
    3;94,95,96;,
    3;97,96,95;,
    3;96,97,98;,
    3;99,98,97;,
    3;98,99,100;,
    3;101,100,99;,
    3;100,101,102;,
    3;74,102,101;,
    3;102,74,73;,
    3;103,104,105;,
    3;104,103,106;,
    3;107,106,103;,
    3;106,107,108;,
    3;109,108,107;,
    3;108,109,110;,
    3;111,110,109;,
    3;110,111,112;,
    3;113,112,111;,
    3;112,113,114;,
    3;115,114,113;,
    3;114,115,116;,
    3;117,116,115;,
    3;116,117,118;,
    3;119,118,117;,
    3;118,119,120;,
    3;121,120,119;,
    3;120,121,122;,
    3;123,122,121;,
    3;122,123,124;,
    3;125,124,123;,
    3;124,125,126;,
    3;127,126,125;,
    3;126,127,128;,
    3;129,128,127;,
    3;128,129,130;,
    3;131,130,129;,
    3;130,131,132;,
    3;133,132,131;,
    3;132,133,134;,
    3;135,134,133;,
    3;134,135,136;,
    3;137,136,135;,
    3;136,137,138;,
    3;105,138,137;,
    3;138,105,104;,
    3;139,140,140;,
    3;140,139,139;,
    3;141,139,139;,
    3;139,141,142;,
    3;143,142,141;,
    3;142,143,144;,
    3;145,144,143;,
    3;144,145,146;,
    3;147,146,145;,
    3;146,147,148;,
    3;149,148,147;,
    3;148,149,82;,
    3;150,82,149;,
    3;82,150,151;,
    3;152,151,150;,
    3;151,152,153;,
    3;154,153,152;,
    3;153,154,155;,
    3;156,155,154;,
    3;155,156,157;,
    3;158,157,156;,
    3;157,158,159;,
    3;160,159,158;,
    3;159,160,161;,
    3;162,161,160;,
    3;161,162,163;,
    3;164,163,162;,
    3;163,164,165;,
    3;166,165,164;,
    3;165,166,97;,
    3;167,97,166;,
    3;97,167,99;,
    3;101,99,167;,
    3;99,101,102;,
    3;140,102,101;,
    3;102,140,140;,
    3;168,169,170;,
    3;171,169,168;,
    3;172,169,171;,
    3;173,169,172;,
    3;174,169,173;,
    3;175,169,174;,
    3;176,169,175;,
    3;177,169,176;,
    3;178,169,177;,
    3;179,169,178;,
    3;180,169,179;,
    3;181,169,180;,
    3;182,169,181;,
    3;183,169,182;,
    3;184,169,183;,
    3;185,169,184;,
    3;186,169,185;,
    3;170,169,186;,
    3;187,187,187;,
    3;187,187,187;,
    3;187,187,187;,
    3;187,187,187;,
    3;187,187,187;,
    3;187,187,187;,
    3;187,187,187;,
    3;187,187,187;,
    3;187,187,187;,
    3;187,187,187;,
    3;187,187,187;,
    3;187,187,187;,
    3;187,187,187;,
    3;187,187,187;,
    3;187,187,187;,
    3;187,187,187;,
    3;187,187,187;,
    3;187,187,187;,
    3;187,187,187;,
    3;187,187,187;,
    3;187,187,187;,
    3;187,187,187;,
    3;187,187,187;,
    3;187,187,187;,
    3;187,187,187;,
    3;187,187,187;,
    3;187,187,187;,
    3;187,187,187;,
    3;187,187,187;,
    3;187,187,187;,
    3;187,187,187;,
    3;187,187,187;,
    3;187,187,187;,
    3;187,187,187;,
    3;187,187,187;,
    3;187,187,187;,
    3;188,189,190;,
    3;189,188,191;,
    3;190,192,193;,
    3;192,190,189;,
    3;193,194,195;,
    3;194,193,192;,
    3;195,196,197;,
    3;196,195,194;,
    3;197,198,199;,
    3;198,197,196;,
    3;199,200,201;,
    3;200,199,198;,
    3;201,202,203;,
    3;202,201,200;,
    3;203,204,205;,
    3;204,203,202;,
    3;205,206,207;,
    3;206,205,204;,
    3;207,208,209;,
    3;208,207,206;,
    3;209,210,211;,
    3;210,209,208;,
    3;211,212,213;,
    3;212,211,210;,
    3;213,214,215;,
    3;214,213,212;,
    3;215,216,217;,
    3;216,215,214;,
    3;217,218,219;,
    3;218,217,216;,
    3;219,220,221;,
    3;220,219,218;,
    3;221,222,223;,
    3;222,221,220;,
    3;223,191,188;,
    3;191,223,222;,
    3;224,225,226;,
    3;226,225,227;,
    3;227,225,228;,
    3;228,225,229;,
    3;229,225,230;,
    3;230,225,231;,
    3;231,225,232;,
    3;232,225,233;,
    3;233,225,234;,
    3;234,225,235;,
    3;235,225,236;,
    3;236,225,237;,
    3;237,225,238;,
    3;238,225,239;,
    3;239,225,240;,
    3;240,225,241;,
    3;241,225,242;,
    3;242,225,224;;
   }

   MeshTextureCoords  {
    154;
    0.750000;0.400000;,
    0.805556;0.400000;,
    0.861111;0.400000;,
    0.916667;0.400000;,
    0.972222;0.400000;,
    1.027778;0.400000;,
    0.083333;0.400000;,
    0.138889;0.400000;,
    0.194444;0.400000;,
    0.250000;0.400000;,
    0.305556;0.400000;,
    0.361111;0.400000;,
    0.416667;0.400000;,
    0.472222;0.400000;,
    0.527778;0.400000;,
    0.583333;0.400000;,
    0.638889;0.400000;,
    0.694444;0.400000;,
    0.750000;0.000000;,
    0.805556;0.000000;,
    0.861111;0.000000;,
    0.916667;0.000000;,
    0.972222;0.000000;,
    1.027778;0.000000;,
    0.083333;0.000000;,
    0.138889;0.000000;,
    0.194444;0.000000;,
    0.250000;0.000000;,
    0.305556;0.000000;,
    0.361111;0.000000;,
    0.416667;0.000000;,
    0.472222;0.000000;,
    0.527778;0.000000;,
    0.583333;0.000000;,
    0.638889;0.000000;,
    0.694444;0.000000;,
    0.750000;0.400000;,
    0.805556;0.400000;,
    0.861111;0.400000;,
    0.916667;0.400000;,
    0.972222;0.400000;,
    1.027778;0.400000;,
    0.083333;0.400000;,
    0.138889;0.400000;,
    0.194444;0.400000;,
    0.250000;0.400000;,
    0.305556;0.400000;,
    0.361111;0.400000;,
    0.416667;0.400000;,
    0.472222;0.400000;,
    0.527778;0.400000;,
    0.583333;0.400000;,
    0.638889;0.400000;,
    0.694444;0.400000;,
    0.750000;0.400000;,
    0.805556;0.400000;,
    0.861111;0.400000;,
    0.916667;0.400000;,
    0.972222;0.400000;,
    1.027778;0.400000;,
    0.083333;0.400000;,
    0.138889;0.400000;,
    0.194444;0.400000;,
    0.250000;0.400000;,
    0.305556;0.400000;,
    0.361111;0.400000;,
    0.416667;0.400000;,
    0.472222;0.400000;,
    0.527778;0.400000;,
    0.583333;0.400000;,
    0.638889;0.400000;,
    0.694444;0.400000;,
    0.750000;0.400000;,
    0.805556;0.400000;,
    0.861111;0.400000;,
    0.916667;0.400000;,
    0.972222;0.400000;,
    1.027778;0.400000;,
    0.083333;0.400000;,
    0.138889;0.400000;,
    0.194444;0.400000;,
    0.250000;0.400000;,
    0.305556;0.400000;,
    0.361111;0.400000;,
    0.416667;0.400000;,
    0.472222;0.400000;,
    0.527778;0.400000;,
    0.583333;0.400000;,
    0.638889;0.400000;,
    0.694444;0.400000;,
    0.750000;0.400000;,
    0.805556;0.400000;,
    0.861111;0.400000;,
    0.916667;0.400000;,
    0.972222;0.400000;,
    1.027778;0.400000;,
    0.083333;0.400000;,
    0.138889;0.400000;,
    0.194444;0.400000;,
    0.250000;0.400000;,
    0.305556;0.400000;,
    0.361111;0.400000;,
    0.416667;0.400000;,
    0.472222;0.400000;,
    0.527778;0.400000;,
    0.583333;0.400000;,
    0.638889;0.400000;,
    0.694444;0.400000;,
    0.527778;0.400000;,
    0.750000;0.000000;,
    0.805556;0.000000;,
    0.861111;0.000000;,
    0.916667;0.000000;,
    0.972222;0.000000;,
    1.027778;0.000000;,
    0.083333;0.000000;,
    0.138889;0.000000;,
    0.194444;0.000000;,
    0.250000;0.000000;,
    0.305556;0.000000;,
    0.361111;0.000000;,
    0.416667;0.000000;,
    0.472222;0.000000;,
    0.527778;0.000000;,
    0.583333;0.000000;,
    0.638889;0.000000;,
    0.694444;0.000000;,
    0.750000;0.000000;,
    0.805556;0.000000;,
    0.861111;0.000000;,
    0.916667;0.000000;,
    0.972222;0.000000;,
    1.027778;0.000000;,
    0.083333;0.000000;,
    0.138889;0.000000;,
    0.194444;0.000000;,
    0.250000;0.000000;,
    0.305556;0.000000;,
    0.361111;0.000000;,
    0.416667;0.000000;,
    0.472222;0.000000;,
    0.527778;0.000000;,
    0.583333;0.000000;,
    0.638889;0.000000;,
    0.694444;0.000000;,
    0.527778;0.000000;,
    0.027778;0.400000;,
    0.027778;0.000000;,
    0.027778;0.400000;,
    0.027778;0.400000;,
    0.027778;0.400000;,
    0.027778;0.400000;,
    0.027778;0.000000;,
    0.027778;0.000000;;
   }
  }
 }
}