"use strict";

function startScene() {
  const scene = document.querySelector("#scene");
  const play = document.querySelector("#play");
  const seek = document.querySelector("#seek");
  const clock = document.querySelector("#clock");
  const viewButtons = Array.from(document.querySelectorAll(".switch button"));
  const perspective = document.querySelector("#perspective");
  const screen = document.querySelector(".screen");
  const screens = document.querySelector(".screens");
  const secondView = document.querySelector("#second-view");
  const frameNumber = document.querySelector("#frame-number");
  const lastFrame = document.querySelector("#last-frame");
  const previous = document.querySelector("#previous-frame");
  const next = document.querySelector("#next-frame");

  const width = scene.width;
  const height = scene.height;
  const scale = Math.min(2, window.devicePixelRatio || 1);
  const layers = [scene, document.createElement("canvas"), document.createElement("canvas"),
                  document.querySelector("#comparison-scene")];
  const contexts = layers.map(canvas => {
    canvas.width = width * scale;
    canvas.height = height * scale;
    const ctx = canvas.getContext("2d");
    ctx.scale(scale, scale);
    return ctx;
  });

  let ctx = contexts[0];
  let gradient;
  let engine;
  let memory;
  const strings = new Map();
  const decoder = new TextDecoder();
  function stringAt(address) {
    if (!strings.has(address)) {
      const end = memory.indexOf(0, address);
      strings.set(address, decoder.decode(memory.subarray(address, end)));
    }
    return strings.get(address);
  }
  const fill = address => address ? stringAt(address) : gradient;

  const canvas = {
    selectLayer(index) { ctx = contexts[index]; },
    drawLayer(index) { ctx.drawImage(layers[index], 0, 0, width, height); },
    save() { ctx.save(); },
    restore() { ctx.restore(); },
    transform(...affine) { ctx.transform(...affine); },
    roundedLines() { ctx.lineCap = "round"; ctx.lineJoin = "round"; },
    opacity(alpha) { ctx.globalAlpha = alpha; },
    line(x1, y1, x2, y2, color, thickness) {
      ctx.beginPath();
      ctx.strokeStyle = stringAt(color);
      ctx.lineWidth = thickness;
      ctx.moveTo(x1, y1);
      ctx.lineTo(x2, y2);
      ctx.stroke();
    },
    ellipse(x, y, rx, ry, color) {
      ctx.beginPath();
      ctx.fillStyle = fill(color);
      ctx.ellipse(x, y, rx, ry, 0, 0, 2 * Math.PI);
      ctx.fill();
    },
    rect(x, y, w, h, color) {
      ctx.fillStyle = fill(color);
      ctx.fillRect(x, y, w, h);
    },
    path(address, count, color, strokeWidth) {
      const points = new Float64Array(memory.buffer, address, count * 2);
      ctx.beginPath();
      ctx.moveTo(points[0], points[1]);
      for (let i = 2; i < points.length; i += 2) ctx.lineTo(points[i], points[i + 1]);
      if (strokeWidth) {
        ctx.strokeStyle = stringAt(color);
        ctx.lineWidth = strokeWidth;
        ctx.stroke();
      } else {
        ctx.closePath();
        ctx.fillStyle = fill(color);
        ctx.fill();
      }
    },
    text(value, x, y, font, color) {
      ctx.fillStyle = stringAt(color);
      ctx.font = stringAt(font);
      ctx.fillText(stringAt(value), x, y);
    },
    linearGradient(...coords) { gradient = ctx.createLinearGradient(...coords); },
    radialGradient(x, y, r1, r2) { gradient = ctx.createRadialGradient(x, y, r1, x, y, r2); },
    colorStop(offset, color) { gradient.addColorStop(offset, stringAt(color)); },
  };

  let frame = 0;
  function nextFrame(now) {
    frame = requestAnimationFrame(nextFrame);
    engine.tick(now / 1000);
  }
  let displayedView = -1;
  function showState(index, endFrame, fps, playing, view) {
    const tenths = Math.floor(index * 10 / fps);
    clock.value = "00:" + String(Math.floor(tenths / 10)).padStart(2, "0") + "." + tenths % 10;
    seek.value = tenths;
    seek.setAttribute("aria-valuetext", (tenths / 10).toLocaleString("vi") + " giây");
    seek.style.setProperty("--progress", (100 * index / endFrame) + "%");
    frameNumber.value = String(index).padStart(3, "0");
    if (lastFrame.textContent !== String(endFrame)) lastFrame.textContent = endFrame;
    previous.disabled = index === 0;
    next.disabled = index === endFrame;

    const action = playing ? "Tạm dừng" : "Phát";
    if (play.title !== action) {
      play.classList.toggle("playing", Boolean(playing));
      play.title = action;
      play.setAttribute("aria-label", action);
    }
    if (displayedView !== view) {
      viewButtons.forEach((button, i) => button.setAttribute("aria-pressed", String(i === view)));
      screen.classList.toggle("esp", view === 1);
      perspective.textContent = view === 1 ? "Góc nhìn của Mực" : "Góc nhìn của Tùng";
      screens.classList.toggle("comparing", view === 2);
      secondView.hidden = view !== 2;
      displayedView = view;
    }
    if (playing && !frame) frame = requestAnimationFrame(nextFrame);
    if (!playing && frame) {
      cancelAnimationFrame(frame);
      frame = 0;
    }
  }

  const bytes = Uint8Array.from(atob(sceneWasmBase64), char => char.charCodeAt(0));
  const module = new WebAssembly.Module(bytes);
  engine = new WebAssembly.Instance(module, {canvas, host: {sin: Math.sin, showState}}).exports;
  memory = new Uint8Array(engine.memory.buffer);
  engine.init();

  const toggle = () => engine.toggle(performance.now() / 1000);
  play.addEventListener("click", toggle);
  document.querySelector("#reset").addEventListener("click", () => engine.reset());
  seek.addEventListener("input", () => engine.seekTo(Number(seek.value) / 10, performance.now() / 1000));
  viewButtons.forEach((button, i) => button.addEventListener("click", () => engine.setView(i)));
  previous.addEventListener("click", () => engine.stepFrame(-1));
  next.addEventListener("click", () => engine.stepFrame(1));
  document.addEventListener("keydown", event => {
    if (event.altKey || event.ctrlKey || event.metaKey || event.repeat) return;
    if (["1", "2", "3"].includes(event.key)) engine.setView(Number(event.key) - 1);
    if (event.key === ",") engine.stepFrame(-1);
    if (event.key === ".") engine.stepFrame(1);
    if (event.code === "Space" && !event.target.closest("button,input")) {
      event.preventDefault();
      toggle();
    }
  });
}

try {
  startScene();
} catch (error) {
  const status = document.querySelector("#perspective");
  status.setAttribute("role", "alert");
  status.textContent = "Lỗi tải bản mô phỏng WebAssembly.";
  for (const control of document.querySelectorAll("button, input")) control.disabled = true;
  console.error(error);
}
