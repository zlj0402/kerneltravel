/**
 * @brief: try to find out: Empty e2 = e1; => copy ctor or copy assignment? A: copy ctor -- 26/3/2025
 *
 * @output: 
 *		sizeof Empty class: 1
 *		>>>>> Empty e1; <<<<<-s
 *		default ctor is called. --- 7fff348da754
 *		>>>>> Empty e1; <<<<<-e
 *		sizeof Empty class' object: 1
 *
 *		>>>>> Empty e2(e1); <<<<<-s
 *		copy ctor is called. --- 7fff348da755
 *		>>>>> Empty e2(e1); <<<<<-e
 *
 *		>>>>> Empty e3; e3 = e1; <<<<<-s
 *		default ctor is called. --- 7fff348da756
 *		copy assignment is called. --- 7fff348da756
 *		>>>>> Empty e3; e3 = e1; <<<<<-e
 *
 *		>>>>> Empty e4 = e1; <<<<<-s
 *		copy ctor is called. --- 7fff348da757
 *		>>>>> Empty e4 = e1; <<<<<-e
 *
 *		----- before return 0 -----
 *		dctor is called. --- 7fff348da757
 *		dctor is called. --- 7fff348da756
 *		dctor is called. --- 7fff348da755
 *		dctor is called. --- 7fff348da754
 *
 * @summary:
 * 		+ About topic's answer, we can know it's copy ctor.
 * 		+ !!! Objects are destroyed in the opposite order of their construction !!!
 */
