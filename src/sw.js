importScripts(
  "https://storage.googleapis.com/workbox-cdn/releases/3.2.0/workbox-sw.js"
);

workbox.clientsClaim();
workbox.skipWaiting();

workbox.precaching.precacheAndRoute([]);

workbox.routing.registerRoute(
  /\.(?:js|css|html)$/,
  workbox.strategies.networkFirst()
);
