#version 110

uniform sampler2D textures[2];

varying vec2 texcoord;

void main()
{
	// For textures blending
    // gl_FragColor = mix(
    //    texture2D(textures[0], texcoord),
    //    texture2D(textures[1], texcoord),
    //    0.0
    // );

    gl_FragColor = texture2D(textures[0], texcoord);
}
