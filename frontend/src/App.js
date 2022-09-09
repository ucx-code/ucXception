import React, { Suspense } from "react";
import RoutePages from "./RoutePages";

function App() {
  return (
    <Suspense fallback={<p>Wait while rendering page...</p>}>
      <RoutePages />
    </Suspense>
  );
}

export default App;
