function OnTrigger(words_on_board)
    local points = 0
    for index, value in ipairs(words_on_board) do
        if #value == 4 then
            points = points + 10
        end
    end

    return points
end