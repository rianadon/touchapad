<script>
 import { onMount } from 'svelte';

 export let date;
 $: formatted = format(date);

 function format(date) {
     const diff = Date.now() - date;
     let fmt;
     if (diff < 5000) {
         fmt = 'just recently';
     } else if (diff < 60000) {
         const secs = Math.round(diff/1000);
         fmt = `${secs} ${secs > 1 ? 'seconds' : 'second'} ago`;
     } else if (diff < 3600*1000) {
         let mins = Math.round(diff/60000);
         fmt = `${mins} ${mins > 1 ? 'minutes' : 'minute'} ago`;
     } else if (diff < 5*3600*1000) {
         fomatted = 'a long time ago';
     } else {
         fmt = 'some time ago';
     }
     return fmt;
 }

 onMount(() => {
     let id;
     function update() {
         formatted = format(date);
         id = setTimeout(update, 5000);
     }
     update();
     return () => clearTimeout(id);
 });
</script>
{formatted}
