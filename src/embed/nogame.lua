--
-- Copyright (c) 2017 rxi, (c) 2019 ShoesForClues
--
-- This library is free software; you can redistribute it and/or modify it
-- under the terms of the MIT license. See LICENSE for details.
--

function love.nogame()
	function love.load()
		love.graphics.setBackgroundColor(0,138,255)
	end

	function love.keypressed(key)
		if key=="escape" then
			love.event.quit()
		end
	end

	local function drawText(str,y)
		local screenw=love.graphics.getWidth()
		local font=love.graphics.getFont()
		love.graphics.print(str,(screenw-font:getWidth(str))/2,y)
	end
	
	local radius=10
	local d=1;
	local r=0;

	function love.draw()
		love.graphics.setColor(0,170,255)
		for y=0,love.graphics.getHeight()/(radius*2) do
			for x=0,love.graphics.getWidth()/(radius*2) do
				if (y*radius*2)%(radius*4)==0 then
					if (x*radius*2)%(radius*4)==0 then
						love.graphics.circle("fill",x*radius*2+radius,y*radius*2+radius,r)
					else
						love.graphics.circle("fill",x*radius*2+radius,y*radius*2+radius,radius-r)
					end
				else
					if (x*radius*2)%(radius*4)~=0 then
						love.graphics.circle("fill",x*radius*2+radius,y*radius*2+radius,r)
					else
						love.graphics.circle("fill",x*radius*2+radius,y*radius*2+radius,radius-r)
					end
				end
			end
		end
		love.graphics.setColor(255,255,255)
		drawText("LoveDOS "..love.getVersion(),love.graphics.getHeight()*0.45)
		drawText("No game",love.graphics.getHeight()*0.45+love.graphics.getFont():getHeight())
		love.graphics.setColor(50,200,255)
		drawText("Press ESCAPE to quit",love.graphics.getHeight()*0.85)
	end
	
	function love.update()
		r=r+d
		if r>=radius then
			d=-1
		elseif r<=0 then
			d=1
		end
		love.timer.sleep(0.05)
	end
end
