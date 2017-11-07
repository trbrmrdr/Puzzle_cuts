#version 120
#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2DRect tex0;
varying vec2 texCoordVarying;

uniform vec2 u_ids;

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;


#define RAD_45 0.7853985
#define RAD_90 1.570797

mat2 rotate(float alpha){
    float _cos=cos(alpha);
    float _sin = sin(alpha);
    return mat2(_cos,_sin, -_sin,_cos);
}

vec3 sphere(float r, float a,float b,vec2 st){
    vec3 col;
    float t = sqrt((st.x*st.x)/(a*a)+(st.y*st.y)/(b*b));
    col =  t>=r ?vec3(.0):vec3(1.);
    return col;
}

vec3 rect(vec2 size,vec2 pos,vec2 st){
    vec3 col;
    
    vec2 t_col =step(pos - size, st) -
        		step(pos + size, st); 
    col = (t_col.x>.0 && t_col.y>.0) ? vec3(1.):vec3(0);
    return col;
}


vec3 yf_paz(vec2 st){
    vec3 col = vec3(.0);
    vec3 rect_col = rect(
        vec2(0.160,(0.108)) ,
        vec2((0.000),(-0.450))+.5, st);
    
    col =  rect_col + sphere(0.412,0.488,.2,st + vec2(-0.480,-0.250)+vec2(-0.200,0.470)*.1);
    
    float r = 0.632;0.520;
    float a = 0.444;0.396;
    float b = 0.176;.2;
    vec2 d_pos = vec2((0.360),(-0.074));
    vec3 t_col =  col - sphere(r,a,b,st + -vec2(.5,.0) + d_pos);
    col =  t_col - sphere(r,a,b,st + -vec2(.5,.0) + vec2(-1.*d_pos.x,d_pos.y));
    
    col = clamp(col,vec3(.0),vec3(1.));
    return col;
}

vec3 yf_paz2(vec2 st){
    vec3 col = vec3(.0);
    
    float r = 0.687;
    float a = 0.357;
    float b = .2;
    vec2 d_pos = vec2(0.325,+0.095);
    vec3 t_col =  col + sphere(r,a,b,st + -vec2(.5,.0) + d_pos);
    col =  t_col + sphere(r,a,b,st + -vec2(.5,.0) + vec2(-1.*d_pos.x,d_pos.y));
    
    col = clamp(col,vec3(.0),vec3(1.));
    return col;
}

vec3 yf_paz2_d(in vec2 st) { return yf_paz2(st); }
vec3 yf_paz2_l(in vec2 st){ return yf_paz2(st*vec2(-1.,1.)*rotate(RAD_90)); }
vec3 yf_paz2_u(in vec2 st){ return yf_paz2(st*vec2(1.,-1.)+vec2(.0,1.)); }
vec3 yf_paz2_r(in vec2 st){ return yf_paz2((st+vec2(-1.,.0))* rotate(RAD_90)); }
//#define COUNT_L 5.
float COUNT_L = 5.;
bool active = true;
vec3 paz(float id_x,float id_y,vec2 st){
    vec3 col = vec3(.0);
    vec3 col_d = yf_paz(st);
    vec3 col_l = yf_paz(st*vec2(-1.,1.)*rotate(RAD_90));
    vec3 col_u = yf_paz(st*vec2(1.,-1.)+vec2(.0,1.));
    vec3 col_r = yf_paz((st+vec2(-1.,.0))* rotate(RAD_90));
    
    #if 1
    if(mod(id_y,2.)>.0 ){
    	if(id_x > 1. && id_x <= COUNT_L) col = col_l; 
        if(id_x >=1. && id_x < COUNT_L ) col += yf_paz2_r(st);
	}
    else{
        if(id_x < COUNT_L && id_x >= 1.) col = col_r; 
        if(id_x <=COUNT_L && id_x > 1. ) col += yf_paz2_l(st);
    }
    if(mod(id_x,2.)>.0){
        if(id_y < COUNT_L && id_y >=1.) col += col_u;
        if(id_y <=COUNT_L && id_y > 1. ) col += yf_paz2_d(st);
    }
    else{
        if(id_y > 1. && id_y <= COUNT_L) col += col_d;
        if(id_y >= 1. && id_y < COUNT_L) col += yf_paz2_u(st);
    }
    col = floor(col);
    
    if(mod(id_x+id_y,2.)>.0){
        col = vec3(1.0)-col;
    }
    
    #else
     
    	//
    	col += col_u;
    	col += col_d;
    	//col = fract(col);
    
    	col += yf_paz2_l(st);
    #endif
    
    
    if(active){
    	if(col.r>.0)
        	col = vec3(1.,.0,.0);
    	else
        	col = vec3(.0,1.,.0);
    }
    //col = col_l + col_d + col_u + col_r;
    return col;
}



float m_LineWidth = 0.03;
float plot(float fx,float uv_y){
  return  smoothstep( fx-m_LineWidth, fx, uv_y) - 
          smoothstep( fx, fx+m_LineWidth, uv_y);
}

vec3 cl = vec3(0.,0.,1.);
vec3 plot_fx(float fx,vec3 oldColor,vec2 uv){
    float pct = plot(fx,uv.y);
    return mix(oldColor,cl,pct);
}



void main() {
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    //st.x *= u_resolution.x/u_resolution.y;
    vec2 mouse = u_mouse/u_resolution;
    st = texCoordVarying/u_resolution;
    
    vec3 color = vec3(0.);
    vec2 id = 1. + floor(st*COUNT_L);
    color = vec3(paz(id.x, id.y, fract(st*COUNT_L)) );
   
#if 1

    //______________
    active = false;
    float d=.285; st*=1.+d*2.; st-=d;
    vec2 ids = floor(mouse * COUNT_L);
    //ids = vec2(3.,1.);
    ids = u_ids;
    id =  1.+ floor(st) + ids;
    vec3 col =vec3(paz(id.x ,id.y, fract(st) ));
    
    if(mod(ids.x + ids.y, 2.)>=.0 ){
        if( (id.y>ids.y+1. || id.y<=ids.y) &&
           	(id.x>ids.x+1. || id.x<=ids.x ) )
            col = vec3(1.) - col;
        col = clamp(col,.0,1.);
    }
    
    vec3 ex_col;
    {
    //____tr
    ex_col += rect(vec2(d), vec2(1.)+d, st);
    ex_col += yf_paz2_u(st + vec2(-1.,0.));
    ex_col += yf_paz2_r(st +vec2(.0,-1.));
    //____tl
    ex_col += rect(vec2(d), vec2(0.-d,1.+d), st);
    ex_col += yf_paz2_u(st + vec2(1.,0.));
    ex_col += yf_paz2_l(st + vec2(0.,-1.));
    //______br
    ex_col += rect(vec2(d), vec2(1.+d,.0-d), st);
    ex_col += yf_paz2_r(st + vec2(.0,1.));
    ex_col += yf_paz2_d(st + vec2(-1,0.));
    //____bl
    ex_col += rect(vec2(d), vec2(0.)-d, st);
    ex_col += yf_paz2_l(st + vec2(0.,1.));
    ex_col += yf_paz2_d(st + vec2(+1,0.));
    }
    
    if(mod(ids.x + ids.y, 2.)>.0 ){    
        col -=ex_col;
     	col = vec3(1.) - col;
    }else
        col +=ex_col;
    
    
    col = clamp(col,.0,1.);
    color = col;
#endif
    //gl_FragColor = vec4(color,1.);return;
    
    float d_x = 1./(COUNT_L);
    vec2 n_st = ((id-1.)/(COUNT_L)) + d_x * (fract(st));
    
    vec4 tex_p = texture2DRect(tex0, n_st  *u_resolution);
    tex_p = vec4(tex_p.rgb, 1.-length(color.rgb));
    
    gl_FragColor = tex_p;
}