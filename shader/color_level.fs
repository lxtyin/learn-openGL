
out vec4 FragColor;

in vec3 frag_pos;

uniform float low_height;
uniform float max_height;
uniform int height_level;

void main() {
    float res = 0, dlt = (max_height - low_height) / height_level;
    for(int i = 0;i < height_level;i++){
        if(frag_pos.y > low_height + dlt * i){
            res += 1.0 / height_level;
        }
    }
    FragColor = vec4(1, res, 0, 1);
}