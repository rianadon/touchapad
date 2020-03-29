<button on:click>
    <slot/>
</button>

<style>
 button:focus {
     outline: none;
     box-shadow: 0 0 0 4px rgba(110,110,110,.5)
 }
 button :global(svg) {
     display: block;
     width: 30px;
     height: 30px;
     margin: 0 auto 0 auto;
 }
 button, button:hover {
     cursor: pointer;
     color: inherit;
     background: #fff;
	 border: 2px solid #eee;
	 padding: 12px;
     margin: 0 12px;
     font-family: inherit;
     font-size: 20px;
     width: 88px;
     height: 88px;
     line-height: 30px;
     transition: background 0.3s ease-in-out;
     text-transform: lowercase;
     border-radius: 6px;
 }

 button:hover {
     background: #f6f6f6;
 }
</style>
