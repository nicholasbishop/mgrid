#version 410

out vec4 FragColor;
in vec3 gFacetNormal;
in vec3 gTriDistance;
in vec4 gPatchDistance;

const vec3 InnerLineColor = vec3(1, 1, 0);
const bool DrawLines = true;

float amplify(float d, float scale, float offset)
{
    d = scale * d + offset;
    d = clamp(d, 0, 1);
    d = 1 - exp2(-2*d*d);
    return d;
}

void main()
{
    vec3 color = vec3(0.5, 0.5, 0.5);

    if (DrawLines) {
        float d1 = min(min(gTriDistance.x, gTriDistance.y), gTriDistance.z);
        float d2 = min(min(min(gPatchDistance.x, gPatchDistance.y), gPatchDistance.z), gPatchDistance.w);
        d1 = 1 - amplify(d1, 50, -0.5);
        d2 = amplify(d2, 50, -0.5);
        color = d2 * color + d1 * d2 * InnerLineColor;
    }

    color *= vec3(1.0) - gFacetNormal;

    FragColor = vec4(color, 1.0);
}
