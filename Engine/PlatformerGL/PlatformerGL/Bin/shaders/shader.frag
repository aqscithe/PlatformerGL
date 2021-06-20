#version 450 core

#define NR_DIR_LIGHTS 00
#define NR_POINT_LIGHTS 00
#define NR_SPOT_LIGHTS 00

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform int DirLightCount;
uniform sampler2D ourTexture;
uniform vec3 camPos;
uniform vec3 modelColor;
uniform bool textureEnabled;
uniform bool outline;
uniform vec3 outlineColor;

struct PointLight {  
    bool enabled;
    vec3 position;
    float attenuation;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constant;
    float linear;
    float quadratic;  
};  

uniform PointLight pointLights[NR_POINT_LIGHTS];

struct DirLight {
    bool enabled;
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
uniform DirLight dirLights[NR_DIR_LIGHTS];

struct SpotLight {
    bool enabled;
    float cutoff;
    vec3 position;
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic; 
};  

uniform SpotLight spotLights[NR_SPOT_LIGHTS];


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);  
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 result = vec3(0.0, 0.0, 0.0);
    vec3 oCol = vec3(1.0, 1.0, 0.0);

    if (outline)
    {
        result = vec3(1.0, 1.0, 1.0);
        FragColor = vec4(result * oCol, 1.0);
    }
    else
    {
        vec3 norm = normalize(Normal);
        vec3 viewDir = normalize(camPos - FragPos);

        // Calculate Directional light
        for (int i = 0; i < NR_DIR_LIGHTS; ++i)
            result += CalcDirLight(dirLights[i], norm, viewDir);

        // Calculate point light color
        for (int i = 0; i < NR_POINT_LIGHTS; ++i)
            result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);

        for (int i = 0; i < NR_SPOT_LIGHTS; ++i)
            result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);

        if (textureEnabled)
            FragColor = vec4(result, 1.0) * texture(ourTexture, TexCoord);
        else
            FragColor = vec4(result * modelColor, 1.0);
    }
	
    
};

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    if (light.enabled)
    {
        vec3 lightDir = normalize(light.position - fragPos);
        float theta = dot(lightDir, normalize(-light.direction));
        if (theta > cos(radians(light.cutoff)))
        {
            // diffuse shading
            float diff = max(dot(normal, lightDir), 0.0);
            // specular shading
            vec3 reflectDir = reflect(-lightDir, normal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

            // attenuation
            float distance    = length(light.position - fragPos);
            float attenuation = 1.0 / (light.constant + light.linear * distance + 
  	    		         light.quadratic * (distance * distance));

            // combine results
            vec3 ambient  = light.ambient;
            vec3 diffuse  = light.diffuse  * diff;
            vec3 specular = light.specular * spec;
            ambient  *= attenuation;
            diffuse  *= attenuation;
            specular *= attenuation;

            return (ambient + diffuse + specular);
        }
        else
            return light.ambient * 0.01;
    }
    return vec3(0.0, 0.0, 0.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    if (light.enabled)
    {
        vec3 lightDir = normalize(-light.direction);
        
        // diffuse shading
        float diff = max(dot(normal, lightDir), 0.0);

        // specular shading
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

        // combine results
        vec3 ambient  = light.ambient;
        vec3 diffuse  = light.diffuse  * diff;
        vec3 specular = light.specular * spec;

        return (ambient + diffuse + specular);
        
    }
    return vec3(0.0, 0.0, 0.0);
    
}  

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    if (light.enabled)
    {
        //light.position -= fragPos;
        vec3 lightDir = normalize(light.position - fragPos);

        // diffuse shading
        float diff = max(dot(normal, lightDir), 0.0);

        // specular shading
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); // 32 - shininess(Ns value in .mtl)

        // attenuation
        float distance    = length(light.position - fragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + 
  	    		     light.quadratic * (distance * distance));    

        // combine results
        vec3 ambient  = light.ambient;            //* vec3(texture(material.diffuse, TexCoords)); (if implemented remove ambientStrength)
        vec3 diffuse  = light.diffuse  * diff;                      //* vec3(texture(material.diffuse, TexCoords));
        vec3 specular = light.specular * spec;   //* vec3(texture(material.specular, TexCoords)); (if implemented remove specularStrength)
        ambient  *= attenuation;
        diffuse  *= attenuation;
        specular *= attenuation;

        return (ambient + diffuse + specular);
    }
    return vec3(0.0, 0.0, 0.0);
    
} 