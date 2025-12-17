#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;      // Ta scène 3D
uniform sampler2D fontTexture;   // Les caractères ASCII
uniform vec2 resolution;         // Taille de l'écran ASCII (160x90)

out vec4 finalColor;

void main()
{
    // Taille d'une cellule de caractère
    vec2 charSize = vec2(8.0, 10.0);

    // Position de la cellule actuelle
    vec2 cellPos = floor(fragTexCoord * resolution / charSize);

    // UV normalisés dans la cellule (0-1)
    vec2 cellUV = fract(fragTexCoord * resolution / charSize);

    // Échantillonner la couleur moyenne de la cellule
    vec2 cellCenter = (cellPos + 0.5) * charSize / resolution;
    vec4 cellColor = texture(texture0, cellCenter);

    // *** PARTIE IMPORTANTE : Convertir en LUMINOSITÉ ***
    // Formule standard de perception humaine de la luminosité
    float lum = dot(cellColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    // Alternative plus simple : float lum = (cellColor.r + cellColor.g + cellColor.b) / 3.0;

    // Nombre de caractères disponibles dans ta palette
    float charCount = 65.0;

    // Sélectionner l'index du caractère basé UNIQUEMENT sur la luminosité
    float charIndex = floor(lum * charCount);

    // UV dans la texture de font (caractères alignés horizontalement)
    vec2 fontUV = vec2((charIndex + cellUV.x) / charCount, cellUV.y);

    // Échantillonner le caractère
    vec4 charPixel = texture(fontTexture, fontUV);

    // *** OPTION 1 : Rendu en noir et blanc pur (basé uniquement sur lumière) ***
    //finalColor = vec4(vec3(lum * charPixel.r), 1.0);

    // *** OPTION 2 : Garder la teinte de couleur mais avec caractères basés sur lumière ***
     finalColor = vec4(cellColor.rgb * charPixel.r, 1.0);
}
