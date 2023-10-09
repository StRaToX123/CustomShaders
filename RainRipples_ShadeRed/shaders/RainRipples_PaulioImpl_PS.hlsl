cbuffer vars : register(b0)
{
	float2 uResolution;
	float uTime;
};

cbuffer RippleDataShaderStructure : register(b1)
{
    float radius;
    float thickness;
    int number_of_ripples;
};

#define SQRT_2 1.41421356237f

float4 main(float4 fragCoord : SV_POSITION) : SV_TARGET
{
    float result = 0.0f;
    // aspectRationCorrection makes sure the ripple is properly scaled 
    // no matter the size of the viewport 
    float2 aspectRationCorrection = float2(uResolution.x / uResolution.y, 1.0f);
    float2 randomOffset = float2(123.456f, 789.012f);
    for (int i = 0; i < number_of_ripples; i++)
    {
        randomOffset = frac(randomOffset * 123.456f);
        float2 color = ((fragCoord.xy / uResolution) - randomOffset) * aspectRationCorrection;
        // Dividing by SQRT_2 in order to normalize the range of colors from 0 to 1
        // the SQRT_2 is the max possible size the ripple can grow to in viewport coords that is 
        // the diagonal od a 1 by 1 right triangle
        color.y = 1.0f - (length(color) / SQRT_2);
        // Limit the radius
        // because the color.x variable falls of away from the pos
        // all values beyond the radius will end up being negative
        // and they will be clamped with the preceding max function.
        // the rest of the values that end up being greater than 0, need to be remaped
        // to a 0 to 1 range by doing / (1.0f - radius)
        color.y = (color.y - radius) / (1.0f - radius);
        // Clamp everything lower than 0 to 0
        color.y = max(0.0f, color.y);
        // Animate the color
        color.x = abs(cos(color.y + (uTime + (uTime * randomOffset.x))));
        // Alter the thickness of the ripple
        color.x /= thickness;
        color.x = min(color.x, 1.0f);
        color.x = 1.0f - color.x;
        // Apply a limit to how far the ripple can grow
        color.x *= color.y;
        result += color.x;
    }
        
    return float4(result, result, result, 1.0f);
}
