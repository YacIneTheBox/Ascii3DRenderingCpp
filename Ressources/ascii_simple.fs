#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;

out vec4 finalColor;

void main()
{
    // Échantillonner la couleur de la texture
    vec4 texColor = texture(texture0, fragTexCoord);

    // Calculer la luminosité
    float lum = dot(texColor.rgb, vec3(0.299, 0.587, 0.114));

    // Palette ASCII de caractères (du plus sombre au plus clair)
    // On simule l'effet en réduisant la résolution et en quantifiant
    vec2 charSize = vec2(8.0, 12.0);
    vec2 screenSize = vec2(640.0, 360.0);

    vec2 charPos = floor(fragTexCoord * screenSize / charSize) * charSize / screenSize;
    vec4 charColor = texture(texture0, charPos);
    float charLum = dot(charColor.rgb, vec3(0.299, 0.587, 0.114));

    // Quantifier la luminosité en niveaux discrets (effet ASCII)
    float levels = 10.0;
    charLum = floor(charLum * levels) / levels;

    finalColor = vec4(vec3(charLum), 1.0) * fragColor;
}
