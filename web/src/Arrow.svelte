<script>
 export let points;

 let d = `M${points[0]} ${points[1]}`
 let horizontal = true
 for (let i = 2; i < points.length; i++) {
     if (points[i] < 1) {
         // Allow fractional points to specify a fraction of the path's length
         points[i] = points[i-2]*points[i] + points[i+2]*(1-points[i]);
     }
     // Figure out the direction headed in the normal and transverse directions
     let ndir = points[i-2] < points[i] ? 1 : -1;
     let tdir = points[i-1] < points[i+1] ? 1 : -1;
     let pi = points[i];
     let pii = points[i-1]
     if (horizontal) {
         // If at the last point, line to the endpoint
         if (i == points.length-1)
             d += ` L${pi} ${pii}`;
         // Otherwise prepare for the next vertical line with a curve
         else
             d += ` L${pi-6*ndir} ${pii}`
                + ` C${pi-4*ndir} ${pii} ${pi} ${pii+2*tdir} ${pi} ${pii+6*tdir}`;
     } else {
         d += ` L${pii} ${pi-6*ndir}`
            + ` C${pii} ${pi-4*ndir} ${pii+2*tdir} ${pi} ${pii+6*tdir} ${pi}`;
     }
     horizontal = !horizontal;
 }
</script>
<path d={d} stroke="#555555" stroke-width="4" stroke-linecap="round"/>
