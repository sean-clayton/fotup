if (process.env.NODE_ENV === "production" && "serviceWorker" in navigator) {
  const x = "/service-worker.js";
  navigator.serviceWorker
    .register(x)
    .then(function(registration) {
      console.log("Registration successful, scope is:", registration.scope);
    })
    .catch(function(error) {
      console.log("Service worker registration failed, error:", error);
    });
}
