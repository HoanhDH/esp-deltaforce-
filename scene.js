"use strict";

const scene = document.querySelector("#scene");
const width = 1200;
const height = 500;
const duration = 16;
const scale = Math.min(2, window.devicePixelRatio || 1);
const pine = "#d9ff97";
const play = document.querySelector("#play");
const seek = document.querySelector("#seek");
const clock = document.querySelector("#clock");
const tung = document.querySelector("#tung");
const muc = document.querySelector("#muc");
let elapsed = 0;
let playing = false;
let esp = false;
let startedAt = 0;
let frame = 0;

function context(canvas) {
  canvas.width = width * scale;
  canvas.height = height * scale;
  const ctx = canvas.getContext("2d");
  ctx.scale(scale, scale);
  return ctx;
}

function layer(paint) {
  const canvas = document.createElement("canvas");
  paint(context(canvas));
  return canvas;
}

function polygon(ctx, points, fill) {
  ctx.fillStyle = fill;
  ctx.beginPath();
  for (const [x, y] of points) ctx.lineTo(x, y);
  ctx.closePath();
  ctx.fill();
}

function line(ctx, x, y, endX, endY, color, thickness = 1) {
  ctx.beginPath();
  ctx.strokeStyle = color;
  ctx.lineWidth = thickness;
  ctx.moveTo(x, y);
  ctx.lineTo(endX, endY);
  ctx.stroke();
}

function rect(ctx, x, y, w, h, color) {
  ctx.fillStyle = color;
  ctx.fillRect(x, y, w, h);
}

function ellipse(ctx, x, y, rx, ry, color) {
  ctx.fillStyle = color;
  ctx.beginPath();
  ctx.ellipse(x, y, rx, ry, 0, 0, 2 * Math.PI);
  ctx.fill();
}

// A fixed seed keeps the scenery identical at every playback position.
function randomSource(seed) {
  return () => {
    seed = (Math.imul(seed, 1664525) + 1013904223) >>> 0;
    return seed / 4294967296;
  };
}

function crane(ctx, x, y, size, color) {
  ctx.save();
  ctx.translate(x, y);
  ctx.scale(size, size);
  const strut = (a,b,c,d,weight) => line(ctx,a,b,c,d,color,weight);
  strut(-22,0,-7,-106,5);
  strut(21,0,6,-106,5);
  for (let i = 0; i < 5; i++) {
    const spread = 19 - i * 2.5;
    strut(-spread,-i*19,spread-2,-(i+1)*19,2);
    strut(spread,-i*19,-spread+2,-(i+1)*19,2);
  }
  strut(-40,-108,99,-146,4);
  strut(-39,-118,99,-149,3);
  for (let i=0; i<12; i++) {
    strut(-39+i*11.5,-118-i*2.55,-28+i*11.5,-111-i*3.1,1.6);
  }
  strut(-29,-118,0,-153,2);
  strut(0,-153,88,-146,1.5);
  strut(0,-153,0,-119,3);
  rect(ctx,-15,-117,23,12,color);
  strut(78,-140,78,-79,1);
  strut(80,-141,80,-79,1);
  rect(ctx,75,-80,8,3,color);
  ctx.restore();
}

function paintHarbor(ctx) {
  const random = randomSource(45);
  const sky = ctx.createLinearGradient(0,0,0,330);
  sky.addColorStop(0,"#48616c");
  sky.addColorStop(.46,"#91a39b");
  sky.addColorStop(.82,"#dfba92");
  sky.addColorStop(1,"#e4c29b");
  rect(ctx,0,0,width,335,sky);
  const glow = ctx.createRadialGradient(866,139,3,866,139,235);
  glow.addColorStop(0,"#ffdcaaa8");
  glow.addColorStop(1,"#f9daaa00");
  rect(ctx,630,0,480,380,glow);
  ellipse(ctx,866,139,35,35,"#f6d9a6");
  for (let i=0; i<65; i++) {
    const x = random()*1240;
    const y = random()*230;
    ellipse(ctx,x,y,8+random()*100,0.5+random()*3.5,"rgba(206,219,200,.08)");
  }
  polygon(ctx,[[0,215],[92,201],[152,208],[250,183],[309,192],[410,168],[437,173],[492,164],[568,188],[630,180],[708,205],[806,178],[889,199],[978,175],[1043,186],[1138,166],[1200,180],[1200,280],[0,280]],"#9da99b");
  for (let x=-24; x<width; x+=18+random()*16) {
    const y=205+random()*33;
    rect(ctx,x,y,24+random()*50,262-y,"#87978c");
    line(ctx,x+7,y,x+7,y-random()*16,"#82958a",1);
  }
  crane(ctx,722,249,.78,"#7b8b80");
  crane(ctx,905,249,.93,"#737f75");
  crane(ctx,1075,250,.70,"#7e8e83");
  rect(ctx,0,246,width,65,"#96a697");
  for (let i=0; i<330; i++) {
    const x=random()*width, y=248+random()*57;
    rect(ctx,x,y,2+random()*45,.5+random(),"rgba(226,213,170,.29)");
  }
  for (let x=0; x<1200; x+=70) {
    rect(ctx,x,281,4,31,"#566a62");
    line(ctx,x,284,x+70,284,"#63776d");
    line(ctx,x,295,x+70,295,"#6e7e71");
  }
  polygon(ctx,[[0,313],[1200,292],[1200,500],[0,500]],"#56625b");
  const ground=ctx.createLinearGradient(0,300,0,500);
  ground.addColorStop(0,"#808779");
  ground.addColorStop(.47,"#52635c");
  ground.addColorStop(1,"#293a37");
  polygon(ctx,[[0,320],[1200,301],[1200,500],[0,500]],ground);
  for(let i=0;i<850;i++) {
    const x=random()*width, y=307+random()*190;
    rect(ctx,x,y,random()*8+.5,random()*1.2+.25,"rgba(209,212,181,.17)");
  }
  for(let i=0;i<7;i++) {
    line(ctx,150+i*210,306,-460+i*310,500,"#40514d",1);
  }
  polygon(ctx,[[1200,355],[1200,361],[0,444],[0,432]],"#ada680");
  polygon(ctx,[[0,151],[186,134],[285,179],[285,324],[0,344]],"#475f5c");
  polygon(ctx,[[186,134],[285,179],[285,324],[186,311]],"#344c4a");
  polygon(ctx,[[0,147],[186,130],[289,177],[284,183],[185,139],[0,156]],"#596e66");
  for(let x=12;x<185;x+=12) line(ctx,x,152-x*.09,x,321-x*.09,"#405653",2);
  for(let x=15;x<170;x+=34) {
    rect(ctx,x,175-x*.08,23,21,"#293e3d");
    line(ctx,x+11,175-x*.08,x+11,196-x*.08,"#73837a",1);
  }
  for(let i=0;i<5;i++) {
    const x=1008+i*36;
    rect(ctx,x,280,29,14,"#736f59");
    rect(ctx,x,280,29,2,"#aaa487");
  }
  for (const x of [303,988]) {
    line(ctx,x,307,x,207,"#445952",3);
    line(ctx,x,208,x+12,202,"#445952",2.5);
    rect(ctx,x+10,201,10,4,"#5d6b5e");
    rect(ctx,x+11,205,8,2,"#f2d7a5");
  }
  for (const [x,y] of [[580,88],[595,82],[614,90]]) {
    line(ctx,x-4,y-1,x,y+1,"#647a76");
    line(ctx,x,y+1,x+4,y-2,"#647a76");
  }
}

function paintContainer(ctx) {
  const random=randomSource(831);
  polygon(ctx,[[278,345],[783,356],[893,389],[324,383]],"rgba(22,38,36,.47)");
  polygon(ctx,[[289,232],[326,215],[788,225],[750,243]],"#a18c72");
  polygon(ctx,[[750,243],[788,225],[787,342],[750,362]],"#5b554c");
  ctx.save();
  ctx.transform(1,.023,0,1,289,232);
  const metal=ctx.createLinearGradient(0,0,0,124);
  metal.addColorStop(0,"#a28b6d");
  metal.addColorStop(.12,"#918068");
  metal.addColorStop(.85,"#83765f");
  metal.addColorStop(1,"#625e52");
  rect(ctx,0,0,461,120,metal);
  for(let x=9;x<455;x+=17) {
    polygon(ctx,[[x,7],[x+4,10],[x+4,109],[x,113]],"#706957");
    rect(ctx,x+4,10,6,99,"#857760");
    line(ctx,x+11,9,x+11,109,"#a48f72",2);
  }
  rect(ctx,0,0,461,4,"#b29b7b");
  rect(ctx,0,115,461,5,"#59594c");
  for(let i=0;i<180;i++) {
    const x=random()*459, y=random()*117;
    rect(ctx,x,y,random()*6+1,1,"rgba(62,62,49,.22)");
  }
  for(const x of [0,452]) {
    rect(ctx,x,0,9,120,"#7c7260");
    for(const y of [3,111]) rect(ctx,x+2,y,4,4,"#464c42");
  }
  ctx.fillStyle="#d3c09c";
  ctx.font="bold 22px monospace";
  ctx.fillText("E-4",17,32);
  for(let i=0;i<3;i++) rect(ctx,20,39+i*3,23-i*4,1,"#c0ad8a");
  ctx.restore();
  for(let x=758;x<786;x+=10) {
    const top=242-(x-750)*.48;
    line(ctx,x,top+4,x,top+111,"#827561",2);
  }
  const edge=ctx.createLinearGradient(0,400,0,500);
  edge.addColorStop(0,"rgba(14,26,26,0)");
  edge.addColorStop(1,"rgba(14,26,26,.62)");
  rect(ctx,0,400,width,100,edge);
}

function walker(ctx, x, t, outline) {
  const stride=Math.sin(t*4.8);
  const lift=Math.abs(stride)*1.5;
  ctx.save();
  ctx.translate(x,336);
  ctx.lineCap="round";
  ctx.lineJoin="round";
  const body=outline ? pine : "#283b39";
  const back=outline ? "#afdc7e" : "#455550";
  const limb=(a,b,c,d,color,weight)=>line(ctx,a,b,c,d,color,weight);
  for(const side of [-1,1]) {
    const swing=stride*side;
    const knee=swing*6+4;
    const foot=swing*13;
    const footY=-2-Math.max(0,-swing)*4;
    const shade=side<0 ? back : body;
    limb(0,-29-lift,knee,-15,shade,6);
    limb(knee,-15,foot,footY,shade,5);
    limb(foot,footY,foot+4,footY,shade,4);
  }
  polygon(ctx,[[-5,-45-lift],[2,-47-lift],[7,-41-lift],[4,-26-lift],[-4,-26-lift]],body);
  ellipse(ctx,3,-52-lift,5,6,body);
  limb(0,-42-lift,-2-stride*5,-35-lift,back,4);
  limb(-2-stride*5,-35-lift,3-stride*8,-31-lift,back,3.5);
  ctx.restore();
}

const ctx=context(scene);
const background=layer(paintHarbor);
const container=layer(paintContainer);

function render() {
  const x=523+elapsed*28;
  ctx.drawImage(background,0,0,width,height);
  ellipse(ctx,x+5,337,21,2.5,"rgba(21,36,32,.28)");
  walker(ctx,x,elapsed,false);
  ctx.drawImage(container,0,0,width,height);
  if(esp) {
    ctx.save();
    ctx.globalAlpha=.92;
    walker(ctx,x,elapsed,true);
    ctx.strokeStyle=pine;
    ctx.lineWidth=1.5;
    for(const [cx,dx] of [[x-22,1],[x+22,-1]]) {
      for(const [cy,dy] of [[269,1],[341,-1]]) {
        ctx.beginPath();
        ctx.moveTo(cx+dx*8,cy);
        ctx.lineTo(cx,cy);
        ctx.lineTo(cx,cy+dy*8);
        ctx.stroke();
      }
    }
    rect(ctx,x-22,249,57,15,"#21372f");
    ctx.fillStyle=pine;
    ctx.font="10px monospace";
    ctx.fillText("NGƯỜI 01",x-18,260);
    ctx.restore();
  }
  const tenths=Math.floor(elapsed*10);
  clock.value="00:"+String(Math.floor(tenths/10)).padStart(2,"0")+"."+tenths%10;
  seek.value=tenths;
  seek.setAttribute("aria-valuetext", (tenths/10).toLocaleString("vi")+" giây");
  seek.style.setProperty("--progress",String(100*elapsed/duration)+"%");
}

function setView(showEsp) {
  esp=showEsp;
  tung.setAttribute("aria-pressed",String(!esp));
  muc.setAttribute("aria-pressed",String(esp));
  document.querySelector(".screen").classList.toggle("esp",esp);
  document.querySelector("#perspective").textContent=esp ? "Góc nhìn của Mực" : "Góc nhìn của Tùng";
  render();
}

function setPlaying(value) {
  playing=value;
  play.classList.toggle("playing",playing);
  play.title=playing ? "Tạm dừng" : "Phát";
  play.setAttribute("aria-label",play.title);
}

function tick(now) {
  elapsed=Math.min(duration,Math.max(0,(now-startedAt)/1000));
  render();
  if(elapsed<duration) frame=requestAnimationFrame(tick);
  else setPlaying(false);
}

function togglePlay() {
  if(playing) {
    cancelAnimationFrame(frame);
    elapsed=Math.min(duration,(performance.now()-startedAt)/1000);
    setPlaying(false);
    render();
    return;
  }
  if(elapsed>=duration) {
    elapsed=0;
    render();
  }
  startedAt=performance.now()-elapsed*1000;
  setPlaying(true);
  frame=requestAnimationFrame(tick);
}

play.addEventListener("click",togglePlay);
document.querySelector("#reset").addEventListener("click",()=>{
  cancelAnimationFrame(frame);
  setPlaying(false);
  elapsed=0;
  render();
});
seek.addEventListener("input",()=>{
  elapsed=Number(seek.value)/10;
  startedAt=performance.now()-elapsed*1000;
  render();
});
tung.addEventListener("click",()=>setView(false));
muc.addEventListener("click",()=>setView(true));
document.addEventListener("keydown",event=>{
  if(event.altKey || event.ctrlKey || event.metaKey || event.repeat) return;
  if(event.key==="1") setView(false);
  if(event.key==="2") setView(true);
  if(event.code==="Space" && !event.target.closest("button,input")) {
    event.preventDefault();
    togglePlay();
  }
});
render();
