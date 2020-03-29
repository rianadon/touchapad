# Frontend code

We have here an app written with Svelte. It has components. These components are:
- `App` :: Lays out the page's interface
- `Arrow` :: Creates an SVG arrow path based on endpoints
- `Button` :: A big button
- `DropdownItem` :: Each item that is shown in a dropdown menu's list of choices
- `Loader` :: Displays a loading icon as data is fetched from the server
- `TimeAgo` :: Converts a date into text like "4 minutes ago"

To launch a server with auto-reloading, use `npm run dev`.

To build a production version of the page, run `npm run build`. You can launch a development server on your machine with `npm start`, or if you have the touchscreen plugged into your computer you can start the app from the `server` directory.
